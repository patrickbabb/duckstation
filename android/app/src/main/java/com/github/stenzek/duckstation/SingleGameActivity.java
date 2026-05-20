package com.github.stenzek.duckstation;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.WindowInsets;
import android.view.WindowInsetsController;
import android.view.WindowManager;
import android.widget.ProgressBar;
import android.widget.TextView;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

public class SingleGameActivity extends Activity {

    private static final String TAG = "SingleGameDebug";

    private ProgressBar progressBar;
    private TextView statusText;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
        getWindow().getDecorView().post(this::applyImmersiveMode);

        setContentView(R.layout.activity_single_game);
        progressBar = findViewById(R.id.progressBar);
        statusText  = findViewById(R.id.statusText);

        new Thread(() -> {

            // 1. Initialize host
            setStatus("Initializing...");
            if (!AndroidHostInterface.hasInstance()) {
                if (!AndroidHostInterface.createInstance(this)) {
                    showFatalError("Failed to create host interface.");
                    return;
                }
            }

            AndroidHostInterface host = AndroidHostInterface.getInstance();
            host.setContext(this);

            String userDirPath = AndroidHostInterface.getUserDirectory();
            File userDir = new File(userDirPath);
            File biosDir = new File(userDir, "bios");
            biosDir.mkdirs();

            // 2. Copy BIOS if missing
            setStatus("Installing BIOS...");
            File biosFile = new File(biosDir, LocalConfig.BIOS_FILE_NAME);
            if (!biosFile.exists()) {
                if (!copyAssetToFile(LocalConfig.BIOS_ASSET_NAME, biosFile)) {
                    showFatalError("Failed to install BIOS.");
                    return;
                }
            }

            if (!host.hasAnyBIOSImages()) {
                showFatalError("BIOS not detected.");
                return;
            }

            // 3. Copy game if missing
            setStatus("Installing game...");
            File gameFile = new File(getExternalFilesDir(null), LocalConfig.GAME_FILE_NAME);
            if (!gameFile.exists()) {
                if (!copyAssetToFile(LocalConfig.GAME_ASSET_NAME, gameFile)) {
                    showFatalError("Failed to install game.");
                    return;
                }
            }

            if (!gameFile.exists()) {
                showFatalError("Game file missing.");
                return;
            }

            // 4. Extract textures if enabled
            if (LocalConfig.HAS_TEXTURES) {
                setStatus("Extracting textures...");
                ensureTexturesExtracted(userDir);
            }

            // 5. Launch
            setStatus("Launching...");
            runOnUiThread(() -> {
                Intent intent = new Intent(this, EmulationActivity.class);
                intent.putExtra("bootPath", gameFile.getAbsolutePath());
                intent.putExtra("resumeState", true);
                startActivity(intent);
                overridePendingTransition(0, 0);
                finish();
            });

        }).start();
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) applyImmersiveMode();
    }

    // ==============================
    // UI HELPERS
    // ==============================

    private void setStatus(String message) {
        runOnUiThread(() -> {
            progressBar.setVisibility(View.VISIBLE);
            statusText.setText(message);
        });
    }

    private void showFatalError(String message) {
        Log.e(TAG, message);
        runOnUiThread(() -> {
            progressBar.setVisibility(View.GONE);
            statusText.setText(message);
        });
    }

    // ==============================
    // IMMERSIVE MODE
    // ==============================

    private void applyImmersiveMode() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            getWindow().setDecorFitsSystemWindows(false);
            getWindow().getAttributes().layoutInDisplayCutoutMode =
                    WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
            getWindow().setAttributes(getWindow().getAttributes());
            WindowInsetsController controller = getWindow().getInsetsController();
            if (controller != null) {
                controller.hide(WindowInsets.Type.systemBars());
                controller.setSystemBarsBehavior(
                        WindowInsetsController.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE);
            }
        } else {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
                WindowManager.LayoutParams lp = getWindow().getAttributes();
                lp.layoutInDisplayCutoutMode =
                        WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
                getWindow().setAttributes(lp);
            }
            //noinspection deprecation
            getWindow().getDecorView().setSystemUiVisibility(
                    View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                            | View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                            | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                            | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                            | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                            | View.SYSTEM_UI_FLAG_FULLSCREEN
            );
        }
    }

    // ==============================
    // TEXTURE EXTRACTION
    // ==============================

    private void ensureTexturesExtracted(File userDir) {
        // DuckStation texture replacement path: <userDir>/textures/<serial>/replacements/
        File targetDir = new File(userDir,
                "textures/" + LocalConfig.GAME_SERIAL + "/replacements");

        String[] contents = targetDir.list();
        if (contents != null && contents.length > 0) return;

        targetDir.mkdirs();

        try {
            String apkPath = getApplicationInfo().sourceDir;
            java.util.zip.ZipFile apk = new java.util.zip.ZipFile(apkPath);

            ZipEntry texturesZipEntry = apk.getEntry("assets/textures.zip");
            if (texturesZipEntry == null) {
                Log.w(TAG, "assets/textures.zip not found in APK — skipping");
                apk.close();
                return;
            }

            try (InputStream zipStream = apk.getInputStream(texturesZipEntry);
                 ZipInputStream zip = new ZipInputStream(zipStream)) {

                ZipEntry entry;
                byte[] buffer = new byte[1024 * 1024];

                while ((entry = zip.getNextEntry()) != null) {
                    File outFile = new File(targetDir, entry.getName()).getCanonicalFile();

                    if (!outFile.getPath().startsWith(targetDir.getCanonicalPath())) {
                        zip.closeEntry();
                        continue;
                    }

                    if (entry.isDirectory()) {
                        outFile.mkdirs();
                    } else {
                        outFile.getParentFile().mkdirs();
                        try (FileOutputStream out = new FileOutputStream(outFile)) {
                            int len;
                            while ((len = zip.read(buffer)) > 0) out.write(buffer, 0, len);
                        }
                    }

                    zip.closeEntry();
                }
            }

            apk.close();

        } catch (Exception e) {
            throw new RuntimeException("Failed to extract textures", e);
        }
    }

    // ==============================
    // ASSET COPY
    // ==============================

    private boolean copyAssetToFile(String assetName, File outFile) {
        try (InputStream in = getAssets().open(assetName);
             FileOutputStream out = new FileOutputStream(outFile)) {
            byte[] buffer = new byte[8192];
            int read;
            while ((read = in.read(buffer)) != -1) {
                out.write(buffer, 0, read);
            }
            return true;
        } catch (Exception e) {
            Log.e(TAG, "Copy failed for " + assetName + ": " + e.getMessage());
            return false;
        }
    }
}