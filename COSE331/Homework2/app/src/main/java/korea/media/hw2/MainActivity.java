package korea.media.hw2;

import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.widget.TextView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    public native void mouseDownEvents(float x, float y);
    public native void mouseMoveEvents(float x, float y);
//    public native void mouseUpEvents(float x, float y, boolean doubleTouch);

    boolean firstTouch = false;

    long time = System.currentTimeMillis();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        /*
        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
        */

        // create OpenGL ES surface view
        GLSurfaceView glSurfaceView = new GLSurfaceView(this);
        glSurfaceView.setEGLContextClientVersion(2);
        glSurfaceView.setRenderer(new GLSurfaceRenderer());

        // screen click event
        glSurfaceView.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent)
            {

                switch (motionEvent.getAction())
                {
                    case MotionEvent.ACTION_DOWN :
                        if(firstTouch && (System.currentTimeMillis() - time) <= 300) {
                            //do stuff here for double tap
                            firstTouch = false;

                        } else {
                            firstTouch = true;

                            time = System.currentTimeMillis();
                        }

                        mouseDownEvents(motionEvent.getX(), motionEvent.getY());
                        break;

                    case MotionEvent.ACTION_MOVE :
                        mouseMoveEvents(motionEvent.getX(), motionEvent.getY());
                        break;

                }
                return true;
            }


        });

        setContentView(glSurfaceView);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native void surfaceCreated(AssetManager assetManager);
    public native void surfaceChanged(int width, int height);
    public native void drawFrame(float deltaTime);

    private class GLSurfaceRenderer implements GLSurfaceView.Renderer {

        // last time attribute for calculating delta time
        private long lastTime = System.currentTimeMillis();

        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {

            // call native method
            surfaceCreated(getAssets());
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {

            // call native method
            surfaceChanged(width, height);
        }

        @Override
        public void onDrawFrame(GL10 gl) {

            // calculate delta time
            long currentTime = System.currentTimeMillis();
            float deltaTime = (float) (currentTime - lastTime) / 1000.0f;
            lastTime = currentTime;

            // call native method
            drawFrame(deltaTime);
        }
    }

    // public native String stringFromJNI();
}
