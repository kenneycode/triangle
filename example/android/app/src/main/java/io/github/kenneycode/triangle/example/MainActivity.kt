package io.github.kenneycode.triangle.example

import android.Manifest
import android.opengl.GLSurfaceView
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class MainActivity : AppCompatActivity() {

    init {
        System.loadLibrary("example")
        System.loadLibrary("triangle")
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        ActivityCompat.requestPermissions(this, arrayOf(Manifest.permission.WRITE_EXTERNAL_STORAGE), 1)
        val glSurfaceView = findViewById<GLSurfaceView>(R.id.glSurfaceView)
        glSurfaceView.setEGLContextClientVersion(3)
        glSurfaceView.setEGLConfigChooser(8, 8, 8, 8, 8, 0)
        glSurfaceView.setRenderer(object : GLSurfaceView.Renderer {

            private var width = 0
            private var height = 0

            override fun onDrawFrame(gl: GL10?) {
                drawFrame(width, height, System.currentTimeMillis())
            }

            override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
                this.width = width
                this.height = height
            }

            override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {

            }

        })
        glSurfaceView.renderMode = GLSurfaceView.RENDERMODE_CONTINUOUSLY
    }

    private external fun drawFrame(width: Int, height: Int, timestamp: Long)

}
