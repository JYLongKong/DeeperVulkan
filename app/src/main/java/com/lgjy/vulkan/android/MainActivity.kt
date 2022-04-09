package com.lgjy.vulkan.android

import android.app.NativeActivity
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle

class MainActivity : NativeActivity() {

    companion object {
        init {
            System.loadLibrary("bn-vulkan-lib")
        }
    }
}
