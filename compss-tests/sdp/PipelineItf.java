
package sdp;

import  integratedtoolkit.types.annotations.Constraints;
import  integratedtoolkit.types.annotations.Method;
import  integratedtoolkit.types.annotations.Parameter;
import  integratedtoolkit.types.annotations.Parameter.Direction;
import  integratedtoolkit.types.annotations.Parameter.Type;

public interface PipelineItf {

    @Constraints(processorCPUCount = 1, memoryPhysicalSize = 0.3f )
    @Method(declaringClass = "sdp.PipelineImpl")
    void kernel(
        @Parameter(type = Type.STRING, direction = Direction.IN)
        String label,
        @Parameter(type = Type.INT, direction = Direction.IN)
        int size,
        @Parameter(type = Type.OBJECT, direction = Direction.INOUT)
        byte[][] out,
        @Parameter(type = Type.OBJECT, direction = Direction.IN)
        byte[][][][] in
    );

}

