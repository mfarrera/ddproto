
package sdp;

public class PipelineImpl {

    public static void kernel(String label, int size, byte[][] out, byte[][][][] in) {
        try {
            System.out.println(label);
            out[0] = new byte[size];
        } catch (Exception e) {
            System.out.println(e.toString());
        }
    }

}

