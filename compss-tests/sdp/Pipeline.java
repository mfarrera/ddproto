
package sdp;

import java.util.HashMap;
import java.util.ArrayList;

import sdp.PipelineImpl;

public class Pipeline {

    public static void main(String[] args) {

        //  Tsnap       = 105.6 s
        //  Nfacet      = 7x7
        //  Nf_max      = 65536 (using 1638)
        //  Tobs        = 21600.0 s (using 105.9 s)
        //  Nmajortotal = 9 (using 1)
        // === Prelude ===
        HashMap<int[], byte[][]> extract_lsm = new HashMap<int[], byte[][]>();
        HashMap<int[], byte[][]> local_sky_model = new HashMap<int[], byte[][]>();
        HashMap<int[], byte[][]> telescope_management = new HashMap<int[], byte[][]>();
        HashMap<int[], byte[][]> visibility_buffer = new HashMap<int[], byte[][]>();
        HashMap<int[], byte[][]> reprojection_predict___ifft = new HashMap<int[], byte[][]>();
        HashMap<int[], byte[][]> telescope_data = new HashMap<int[], byte[][]>();
        HashMap<int[], byte[][]> degridding_kernel_update___degrid = new HashMap<int[], byte[][]>();
        HashMap<int[], byte[][]> phase_rotation_predict___dft___sum_visibilities = new HashMap<int[], byte[][]>();
        HashMap<int[], byte[][]> solve = new HashMap<int[], byte[][]>();
        HashMap<int[], byte[][]> correct___subtract_visibility___flag = new HashMap<int[], byte[][]>();
        HashMap<int[], byte[][]> gridding_kernel_update___phase_rotation___grid___fft___reprojection = new HashMap<int[], byte[][]>();
        HashMap<int[], byte[][]> identify_component = new HashMap<int[], byte[][]>();
        HashMap<int[], byte[][]> source_find = new HashMap<int[], byte[][]>();
        HashMap<int[], byte[][]> subtract_image_component = new HashMap<int[], byte[][]>();
        HashMap<int[], byte[][]> update_lsm = new HashMap<int[], byte[][]>();
        // === Extract_LSM ===
        for (int beam = 0; beam < 1; beam++) {
            for (int major_loop = 0; major_loop < 1; major_loop++) {
                byte[][] extract_lsm_out = { null };
                extract_lsm.put(new int[]{beam, major_loop}, extract_lsm_out);
                PipelineImpl.kernel("Extract_LSM (0.0 MB, full 0.0 MB, 0.00 TFLOPs)", 0, extract_lsm_out, new byte[][][][]{});
            }
        }
        // === Local Sky Model ===
        byte[][] local_sky_model_out = { null };
        local_sky_model.put(new int[]{}, local_sky_model_out);
        PipelineImpl.kernel("Local Sky Model (0.0 MB, full 0.0 MB, 0.00 TFLOPs)", 0, local_sky_model_out, new byte[][][][]{});
        // === Telescope Management ===
        byte[][] telescope_management_out = { null };
        telescope_management.put(new int[]{}, telescope_management_out);
        PipelineImpl.kernel("Telescope Management (0.0 MB, full 0.0 MB, 0.00 TFLOPs)", 0, telescope_management_out, new byte[][][][]{});
        // === Visibility Buffer ===
        for (int beam = 0; beam < 1; beam++) {
            for (int frequency = 0; frequency < 20; frequency++) {
                for (int time = 0; time < 1; time++) {
                    for (int baseline = 0; baseline < 1; baseline++) {
                        for (int polarisation = 0; polarisation < 1; polarisation++) {
                            byte[][] visibility_buffer_out = { null };
                            visibility_buffer.put(new int[]{beam, frequency, time, baseline, polarisation}, visibility_buffer_out);
                            PipelineImpl.kernel("Visibility Buffer (190.9 MB, full 57276.3 MB, 0.00 TFLOPs)", 190920857, visibility_buffer_out, new byte[][][][]{});
                        }
                    }
                }
            }
        }
        // === Reprojection Predict + IFFT ===
        for (int beam = 0; beam < 1; beam++) {
            for (int major_loop = 0; major_loop < 1; major_loop++) {
                for (int frequency = 0; frequency < 5; frequency++) {
                    for (int time = 0; time < 1; time++) {
                        for (int facet = 0; facet < 49; facet++) {
                            for (int polarisation = 0; polarisation < 4; polarisation++) {
                                ArrayList<byte[][]> input_extract_lsm_0 = new ArrayList<byte[][]>();
                                input_extract_lsm_0.add(extract_lsm.get(new int[]{beam, major_loop}));
                                byte[][] reprojection_predict___ifft_out = { null };
                                reprojection_predict___ifft.put(new int[]{beam, major_loop, frequency, time, facet, polarisation}, reprojection_predict___ifft_out);
                                PipelineImpl.kernel("Reprojection Predict + IFFT (40.3 MB, full 12104.9 MB, 0.39 TFLOPs)", 40349611, reprojection_predict___ifft_out, new byte[][][][]{input_extract_lsm_0.toArray(new byte[][][]{})});
                            }
                        }
                    }
                }
            }
        }
        // === Telescope Data ===
        for (int beam = 0; beam < 1; beam++) {
            for (int frequency = 0; frequency < 1; frequency++) {
                for (int time = 0; time < 1; time++) {
                    for (int baseline = 0; baseline < 1; baseline++) {
                        ArrayList<byte[][]> input_telescope_management_0 = new ArrayList<byte[][]>();
                        input_telescope_management_0.add(telescope_management.get(new int[]{}));
                        byte[][] telescope_data_out = { null };
                        telescope_data.put(new int[]{beam, frequency, time, baseline}, telescope_data_out);
                        PipelineImpl.kernel("Telescope Data (0.0 MB, full 0.0 MB, 0.00 TFLOPs)", 0, telescope_data_out, new byte[][][][]{input_telescope_management_0.toArray(new byte[][][]{})});
                    }
                }
            }
        }
        // === Degridding Kernel Update + Degrid ===
        for (int beam = 0; beam < 1; beam++) {
            for (int major_loop = 0; major_loop < 1; major_loop++) {
                for (int frequency = 0; frequency < 20; frequency++) {
                    for (int time = 0; time < 1; time++) {
                        for (int facet = 0; facet < 49; facet++) {
                            for (int polarisation = 0; polarisation < 4; polarisation++) {
                                ArrayList<byte[][]> input_telescope_data_0 = new ArrayList<byte[][]>();
                                input_telescope_data_0.add(telescope_data.get(new int[]{beam, frequency*1/20, time, 0}));
                                ArrayList<byte[][]> input_reprojection_predict___ifft_0 = new ArrayList<byte[][]>();
                                input_reprojection_predict___ifft_0.add(reprojection_predict___ifft.get(new int[]{beam, major_loop, frequency*5/20, time, facet, polarisation}));
                                byte[][] degridding_kernel_update___degrid_out = { null };
                                degridding_kernel_update___degrid.put(new int[]{beam, major_loop, frequency, time, facet, polarisation}, degridding_kernel_update___degrid_out);
                                PipelineImpl.kernel("Degridding Kernel Update + Degrid (0.4 MB, full 110.2 MB, 0.07 TFLOPs)", 367350, degridding_kernel_update___degrid_out, new byte[][][][]{input_telescope_data_0.toArray(new byte[][][]{}), input_reprojection_predict___ifft_0.toArray(new byte[][][]{})});
                            }
                        }
                    }
                }
            }
        }
        // === Phase Rotation Predict + DFT + Sum visibilities ===
        for (int beam = 0; beam < 1; beam++) {
            for (int major_loop = 0; major_loop < 1; major_loop++) {
                for (int frequency = 0; frequency < 20; frequency++) {
                    for (int time = 0; time < 1; time++) {
                        for (int baseline = 0; baseline < 1; baseline++) {
                            for (int polarisation = 0; polarisation < 1; polarisation++) {
                                ArrayList<byte[][]> input_degridding_kernel_update___degrid_0 = new ArrayList<byte[][]>();
                                for (int i_facet = 0; i_facet < 49; i_facet++) {
                                    for (int i_polarisation = 0; i_polarisation < 4; i_polarisation++) {
                                        input_degridding_kernel_update___degrid_0.add(degridding_kernel_update___degrid.get(new int[]{beam, major_loop, frequency, time, i_facet, polarisation*4/1+i_polarisation}));
                                    }
                                }
                                ArrayList<byte[][]> input_extract_lsm_0 = new ArrayList<byte[][]>();
                                input_extract_lsm_0.add(extract_lsm.get(new int[]{beam, major_loop}));
                                byte[][] phase_rotation_predict___dft___sum_visibilities_out = { null };
                                phase_rotation_predict___dft___sum_visibilities.put(new int[]{beam, major_loop, frequency, time, baseline, polarisation}, phase_rotation_predict___dft___sum_visibilities_out);
                                PipelineImpl.kernel("Phase Rotation Predict + DFT + Sum visibilities (190.9 MB, full 57276.3 MB, 162.62 TFLOPs)", 190920857, phase_rotation_predict___dft___sum_visibilities_out, new byte[][][][]{input_degridding_kernel_update___degrid_0.toArray(new byte[][][]{}), input_extract_lsm_0.toArray(new byte[][][]{})});
                            }
                        }
                    }
                }
            }
        }
        // === Solve ===
        for (int beam = 0; beam < 1; beam++) {
            for (int major_loop = 0; major_loop < 1; major_loop++) {
                for (int frequency = 0; frequency < 1; frequency++) {
                    for (int time = 0; time < 10; time++) {
                        for (int polarisation = 0; polarisation < 1; polarisation++) {
                            ArrayList<byte[][]> input_visibility_buffer_0 = new ArrayList<byte[][]>();
                            for (int i_frequency = 0; i_frequency < 20; i_frequency++) {
                                input_visibility_buffer_0.add(visibility_buffer.get(new int[]{beam, frequency*20/1+i_frequency, time*1/10, 0, polarisation}));
                            }
                            ArrayList<byte[][]> input_phase_rotation_predict___dft___sum_visibilities_0 = new ArrayList<byte[][]>();
                            for (int i_frequency = 0; i_frequency < 20; i_frequency++) {
                                input_phase_rotation_predict___dft___sum_visibilities_0.add(phase_rotation_predict___dft___sum_visibilities.get(new int[]{beam, major_loop, frequency*20/1+i_frequency, time*1/10, 0, polarisation}));
                            }
                            byte[][] solve_out = { null };
                            solve.put(new int[]{beam, major_loop, frequency, time, polarisation}, solve_out);
                            PipelineImpl.kernel("Solve (27.5 MB, full 8262.8 MB, 485.01 TFLOPs)", 27542596, solve_out, new byte[][][][]{input_visibility_buffer_0.toArray(new byte[][][]{}), input_phase_rotation_predict___dft___sum_visibilities_0.toArray(new byte[][][]{})});
                        }
                    }
                }
            }
        }
        // === Correct + Subtract Visibility + Flag ===
        for (int beam = 0; beam < 1; beam++) {
            for (int major_loop = 0; major_loop < 1; major_loop++) {
                for (int frequency = 0; frequency < 20; frequency++) {
                    for (int time = 0; time < 1; time++) {
                        for (int baseline = 0; baseline < 1; baseline++) {
                            for (int polarisation = 0; polarisation < 1; polarisation++) {
                                ArrayList<byte[][]> input_solve_0 = new ArrayList<byte[][]>();
                                for (int i_time = 0; i_time < 10; i_time++) {
                                    input_solve_0.add(solve.get(new int[]{beam, major_loop, frequency*1/20, time*10/1+i_time, polarisation}));
                                }
                                ArrayList<byte[][]> input_visibility_buffer_0 = new ArrayList<byte[][]>();
                                input_visibility_buffer_0.add(visibility_buffer.get(new int[]{beam, frequency, time, baseline, polarisation}));
                                ArrayList<byte[][]> input_phase_rotation_predict___dft___sum_visibilities_0 = new ArrayList<byte[][]>();
                                input_phase_rotation_predict___dft___sum_visibilities_0.add(phase_rotation_predict___dft___sum_visibilities.get(new int[]{beam, major_loop, frequency, time, baseline, polarisation}));
                                byte[][] correct___subtract_visibility___flag_out = { null };
                                correct___subtract_visibility___flag.put(new int[]{beam, major_loop, frequency, time, baseline, polarisation}, correct___subtract_visibility___flag_out);
                                PipelineImpl.kernel("Correct + Subtract Visibility + Flag (190.9 MB, full 57276.3 MB, 1.52 TFLOPs)", 190920857, correct___subtract_visibility___flag_out, new byte[][][][]{input_solve_0.toArray(new byte[][][]{}), input_visibility_buffer_0.toArray(new byte[][][]{}), input_phase_rotation_predict___dft___sum_visibilities_0.toArray(new byte[][][]{})});
                            }
                        }
                    }
                }
            }
        }
        // === Gridding Kernel Update + Phase Rotation + Grid + FFT + Reprojection ===
        for (int beam = 0; beam < 1; beam++) {
            for (int major_loop = 0; major_loop < 1; major_loop++) {
                for (int frequency = 0; frequency < 1; frequency++) {
                    for (int time = 0; time < 1; time++) {
                        for (int facet = 0; facet < 49; facet++) {
                            for (int polarisation = 0; polarisation < 4; polarisation++) {
                                ArrayList<byte[][]> input_correct___subtract_visibility___flag_0 = new ArrayList<byte[][]>();
                                for (int i_frequency = 0; i_frequency < 20; i_frequency++) {
                                    input_correct___subtract_visibility___flag_0.add(correct___subtract_visibility___flag.get(new int[]{beam, major_loop, frequency*20/1+i_frequency, time, 0, polarisation*1/4}));
                                }
                                ArrayList<byte[][]> input_telescope_data_0 = new ArrayList<byte[][]>();
                                input_telescope_data_0.add(telescope_data.get(new int[]{beam, frequency, time, 0}));
                                byte[][] gridding_kernel_update___phase_rotation___grid___fft___reprojection_out = { null };
                                gridding_kernel_update___phase_rotation___grid___fft___reprojection.put(new int[]{beam, major_loop, frequency, time, facet, polarisation}, gridding_kernel_update___phase_rotation___grid___fft___reprojection_out);
                                PipelineImpl.kernel("Gridding Kernel Update + Phase Rotation + Grid + FFT + Reprojection (40.3 MB, full 12104.4 MB, 2.37 TFLOPs)", 40348144, gridding_kernel_update___phase_rotation___grid___fft___reprojection_out, new byte[][][][]{input_correct___subtract_visibility___flag_0.toArray(new byte[][][]{}), input_telescope_data_0.toArray(new byte[][][]{})});
                            }
                        }
                    }
                }
            }
        }
        // === Identify Component ===
        for (int beam = 0; beam < 1; beam++) {
            for (int major_loop = 0; major_loop < 1; major_loop++) {
                ArrayList<byte[][]> input_gridding_kernel_update___phase_rotation___grid___fft___reprojection_0 = new ArrayList<byte[][]>();
                for (int i_facet = 0; i_facet < 49; i_facet++) {
                    for (int i_polarisation = 0; i_polarisation < 4; i_polarisation++) {
                        input_gridding_kernel_update___phase_rotation___grid___fft___reprojection_0.add(gridding_kernel_update___phase_rotation___grid___fft___reprojection.get(new int[]{beam, major_loop, 0, 0, i_facet, i_polarisation}));
                    }
                }
                byte[][] identify_component_out = { null };
                identify_component.put(new int[]{beam, major_loop}, identify_component_out);
                PipelineImpl.kernel("Identify Component (0.0 MB, full 0.0 MB, 11862.35 TFLOPs)", 0, identify_component_out, new byte[][][][]{input_gridding_kernel_update___phase_rotation___grid___fft___reprojection_0.toArray(new byte[][][]{})});
            }
        }
        // === Source Find ===
        for (int beam = 0; beam < 1; beam++) {
            for (int major_loop = 0; major_loop < 1; major_loop++) {
                ArrayList<byte[][]> input_identify_component_0 = new ArrayList<byte[][]>();
                input_identify_component_0.add(identify_component.get(new int[]{beam, major_loop}));
                byte[][] source_find_out = { null };
                source_find.put(new int[]{beam, major_loop}, source_find_out);
                PipelineImpl.kernel("Source Find (0.1 MB, full 17.3 MB, 0.00 TFLOPs)", 57600, source_find_out, new byte[][][][]{input_identify_component_0.toArray(new byte[][][]{})});
            }
        }
        // === Subtract Image Component ===
        for (int beam = 0; beam < 1; beam++) {
            for (int major_loop = 0; major_loop < 1; major_loop++) {
                ArrayList<byte[][]> input_identify_component_0 = new ArrayList<byte[][]>();
                input_identify_component_0.add(identify_component.get(new int[]{beam, major_loop}));
                byte[][] subtract_image_component_out = { null };
                subtract_image_component.put(new int[]{beam, major_loop}, subtract_image_component_out);
                PipelineImpl.kernel("Subtract Image Component (0.3 MB, full 83.9 MB, 6.71 TFLOPs)", 279756, subtract_image_component_out, new byte[][][][]{input_identify_component_0.toArray(new byte[][][]{})});
            }
        }
        // === Update LSM ===
        for (int beam = 0; beam < 1; beam++) {
            for (int major_loop = 0; major_loop < 1; major_loop++) {
                ArrayList<byte[][]> input_local_sky_model_0 = new ArrayList<byte[][]>();
                input_local_sky_model_0.add(local_sky_model.get(new int[]{}));
                ArrayList<byte[][]> input_source_find_0 = new ArrayList<byte[][]>();
                input_source_find_0.add(source_find.get(new int[]{beam, major_loop}));
                byte[][] update_lsm_out = { null };
                update_lsm.put(new int[]{beam, major_loop}, update_lsm_out);
                PipelineImpl.kernel("Update LSM (0.0 MB, full 0.0 MB, 0.00 TFLOPs)", 0, update_lsm_out, new byte[][][][]{input_local_sky_model_0.toArray(new byte[][][]{}), input_source_find_0.toArray(new byte[][][]{})});
            }
        }
        // === Terminate ===
        ArrayList<byte[][]> input_subtract_image_component_0 = new ArrayList<byte[][]>();
        ArrayList<byte[][]> input_update_lsm_0 = new ArrayList<byte[][]>();
    }
}
//  * 5187 tasks
//  * 60.78 GB produced (nerfed from 18235.01 GB)
//  * 21.41 PFLOP represented
// Assuming 10% algorithmic efficiency this is roughly(!):
//  * 57.56 min node time
//  * 61.18 s island time
//  * 1.53 s cluster time
//  * 0.00708% SKA SDP
//  * 2.36e-05% SKA SDP internal data rate
