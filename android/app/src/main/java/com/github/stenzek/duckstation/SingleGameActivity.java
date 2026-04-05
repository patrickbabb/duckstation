package com.github.stenzek.duckstation;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;

public class SingleGameActivity extends Activity {

    private static final String TAG = "SingleGameDebug";

    private static final String BIOS_ASSET_NAME = "PSXONPSP660.bin";// must match asset name
    private static final String GAME_ASSET_NAME = "game.pbp";     // must match asset name

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Log.i(TAG, "==== SingleGameActivity Started ====");

        // 1️⃣ Initialize host
        if (!AndroidHostInterface.hasInstance()) {
            if (!AndroidHostInterface.createInstance(this)) {
                throw new RuntimeException("Failed to create host interface");
            }
        }

        AndroidHostInterface host = AndroidHostInterface.getInstance();
        host.setContext(this);

        // 2️⃣ Print user directory
        String userDirPath = AndroidHostInterface.getUserDirectory();
        Log.i(TAG, "UserDir: " + userDirPath);

        File userDir = new File(userDirPath);
        File biosDir = new File(userDir, "bios");

        boolean mkdirResult = biosDir.mkdirs();
        Log.i(TAG, "biosDir.mkdirs() result: " + mkdirResult);
        Log.i(TAG, "biosDir exists: " + biosDir.exists());
        Log.i(TAG, "biosDir absolute path: " + biosDir.getAbsolutePath());

        // 3️⃣ Copy BIOS if missing
        File biosFile = new File(biosDir, BIOS_ASSET_NAME);

        Log.i(TAG, "Expected BIOS path: " + biosFile.getAbsolutePath());
        Log.i(TAG, "BIOS exists before copy: " + biosFile.exists());

        if (!biosFile.exists()) {
            boolean copied = copyAssetToFile(BIOS_ASSET_NAME, biosFile);
            Log.i(TAG, "BIOS copied result: " + copied);
        }

        Log.i(TAG, "BIOS exists after copy: " + biosFile.exists());
        Log.i(TAG, "BIOS file size: " + biosFile.length());

        // 4️⃣ Ask native if it sees BIOS
        boolean nativeSeesBios = host.hasAnyBIOSImages();
        Log.i(TAG, "Native sees BIOS: " + nativeSeesBios);

        if (!nativeSeesBios) {
            Log.e(TAG, "Native cannot detect BIOS. Stopping.");
            return;
        }

        // 5️⃣ Ensure game file exists
        File gameFile = new File(getExternalFilesDir(null), GAME_ASSET_NAME);

        Log.i(TAG, "Expected game path: " + gameFile.getAbsolutePath());
        Log.i(TAG, "Game exists before copy: " + gameFile.exists());

        if (!gameFile.exists()) {
            boolean gameCopied = copyAssetToFile(GAME_ASSET_NAME, gameFile);
            Log.i(TAG, "Game copied result: " + gameCopied);
        }

        Log.i(TAG, "Game exists after copy: " + gameFile.exists());
        Log.i(TAG, "Game file size: " + gameFile.length());

        if (!gameFile.exists()) {
            Log.e(TAG, "Game missing. Stopping.");
            return;
        }

        // 6️⃣ Launch emulation
        Log.i(TAG, "Launching EmulationActivity...");
        Intent intent = new Intent(this, EmulationActivity.class);
        intent.putExtra("bootPath", gameFile.getAbsolutePath());
        intent.putExtra("resumeState", true);

        startActivity(intent);
        finish();
    }

    private boolean copyAssetToFile(String assetName, File outFile) {
        try {
            InputStream in = getAssets().open(assetName);
            FileOutputStream out = new FileOutputStream(outFile);

            byte[] buffer = new byte[8192];
            int read;

            while ((read = in.read(buffer)) != -1) {
                out.write(buffer, 0, read);
            }

            in.close();
            out.close();
            return true;

        } catch (Exception e) {
            Log.e(TAG, "Copy failed: " + e.getMessage());
            e.printStackTrace();
            return false;
        }
    }
}
