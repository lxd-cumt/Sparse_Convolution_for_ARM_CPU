#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <sys/times.h>

#include "arm_compute/core/Types.h"
#include "arm_compute/runtime/CL/CLFunctions.h"
#include "arm_compute/runtime/NEON/NEFunctions.h"
#include "arm_compute/runtime/NEON/NEScheduler.h"
#include "arm_compute/runtime/Allocator.h"
#include "utils/ImageLoader.h"
#include "utils/Utils.h"
#include <ctime>
#include <cstdlib>


using namespace arm_compute;
using namespace utils;

class NEONVGG16Example : public Example
{
public:
bool do_setup(int argc, char **argv) override
	{
        string data_path="/media/sdcard/ComputeLibrary/data/neon_vgg_float/";
        NPYLoader npy_input; npy_input.open(data_path+"input.npy");npy_input.init_tensor(input, DataType::F32);
		// const TensorShape input_shape(224, 224, 3);
        // input.allocator()->init(TensorInfo(input_shape, 1, DataType::F32));
		
        NPYLoader npy_weights_conv1, npy_bias_conv1;
        npy_weights_conv1.open(weights_datapath+weights_name[0]); npy_weights_conv1.init_tensor(weights_conv1, DataType::F32);
        npy_bias_conv1   .open(bias_datapath   +bias_name[0]   ); npy_bias_conv1   .init_tensor(bias_conv1   , DataType::F32);
		// const TensorShape weights_conv1_shape(3,3,3,64);
        // weights_conv1.allocator()->init(TensorInfo(weights_conv1_shape, 1, DataType::F32));
        // const TensorShape bias_conv1_shape(64);
        // bias_conv1.allocator()->init(TensorInfo(bias_conv1_shape, 1, DataType::F32));
		const TensorShape out_shape_conv1(224, 224, 64);
		out_conv1.allocator()->init(TensorInfo(out_shape_conv1, 1, DataType::F32));
		out_act1.allocator()->init(TensorInfo(out_shape_conv1,1,DataType:: F32));

		
        NPYLoader npy_weights_conv2, npy_bias_conv2;
        npy_weights_conv2.open(weights_datapath+weights_name[1]); npy_weights_conv2.init_tensor(weights_conv2, DataType::F32);
        npy_bias_conv2   .open(bias_datapath   +bias_name[1]   ); npy_bias_conv2   .init_tensor(bias_conv2   , DataType::F32);
        // const TensorShape weights_conv2_shape(3,3,64,64);
        // weights_conv2.allocator()->init(TensorInfo(weights_conv2_shape, 1, DataType::F32));
        // const TensorShape bias_conv2_shape(64);
        // bias_conv2.allocator()->init(TensorInfo(bias_conv2_shape, 1, DataType::F32));
		const TensorShape out_shape_conv2(224, 224, 64);
		out_conv2.allocator()->init(TensorInfo(out_shape_conv2, 1, DataType:: F32));
		out_act2.allocator()->init(TensorInfo(out_shape_conv2,1,DataType:: F32));
		TensorShape out_shape_pool1 = out_shape_conv2;
		out_shape_pool1.set(0, out_shape_pool1.x() / 2);
		out_shape_pool1.set(1, out_shape_pool1.y() / 2);
		out_pool1.allocator()->init(TensorInfo(out_shape_pool1, 1, DataType:: F32));      


        NPYLoader npy_weights_conv3, npy_bias_conv3;
        npy_weights_conv3.open(weights_datapath+weights_name[2]); npy_weights_conv3.init_tensor(weights_conv3, DataType::F32);
        npy_bias_conv3   .open(bias_datapath   +bias_name[2]   ); npy_bias_conv3   .init_tensor(bias_conv3   , DataType::F32);
		// const TensorShape weights_conv3_shape(3,3,64,128);
        // weights_conv3.allocator()->init(TensorInfo(weights_conv3_shape, 1, DataType::F32));
        // const TensorShape bias_conv3_shape(128);
        // bias_conv3.allocator()->init(TensorInfo(bias_conv3_shape, 1, DataType::F32));
		const TensorShape out_shape_conv3(112, 112, 128);
		out_conv3.allocator()->init(TensorInfo(out_shape_conv3, 1, DataType:: F32));
		out_act3.allocator()->init(TensorInfo(out_shape_conv3,1,DataType:: F32));


        NPYLoader npy_weights_conv4, npy_bias_conv4;
        npy_weights_conv4.open(weights_datapath+weights_name[3]); npy_weights_conv4.init_tensor(weights_conv4, DataType::F32);
        npy_bias_conv4   .open(bias_datapath   +bias_name[3]   ); npy_bias_conv4   .init_tensor(bias_conv4   , DataType::F32);
		// const TensorShape weights_conv4_shape(3,3,128,128);
        // weights_conv4.allocator()->init(TensorInfo(weights_conv4_shape, 1, DataType::F32));
        // const TensorShape bias_conv4_shape(128);
        // bias_conv4.allocator()->init(TensorInfo(bias_conv4_shape, 1, DataType::F32));
		const TensorShape out_shape_conv4(112, 112, 128);
		out_conv4.allocator()->init(TensorInfo(out_shape_conv4, 1, DataType:: F32));
		out_act4.allocator()->init(TensorInfo(out_shape_conv4,1,DataType:: F32));
		TensorShape out_shape_pool2 = out_shape_conv4;
		out_shape_pool2.set(0, out_shape_pool2.x() / 2);
		out_shape_pool2.set(1, out_shape_pool2.y() / 2);
		out_pool2.allocator()->init(TensorInfo(out_shape_pool2, 1, DataType:: F32));      


        NPYLoader npy_weights_conv5, npy_bias_conv5;
        npy_weights_conv5.open(weights_datapath+weights_name[4]); npy_weights_conv5.init_tensor(weights_conv5, DataType::F32);
        npy_bias_conv5   .open(bias_datapath   +bias_name[4]   ); npy_bias_conv5   .init_tensor(bias_conv5   , DataType::F32);
		// const TensorShape weights_conv5_shape(3,3,128,256);
        // weights_conv5.allocator()->init(TensorInfo(weights_conv5_shape, 1, DataType::F32));
        // const TensorShape bias_conv5_shape(256);
        // bias_conv5.allocator()->init(TensorInfo(bias_conv5_shape, 1, DataType::F32));
		const TensorShape out_shape_conv5(56, 56, 256);
		out_conv5.allocator()->init(TensorInfo(out_shape_conv5, 1, DataType:: F32));
		out_act5.allocator()->init(TensorInfo(out_shape_conv5,1,DataType:: F32));


        NPYLoader npy_weights_conv6, npy_bias_conv6;
        npy_weights_conv6.open(weights_datapath+weights_name[5]); npy_weights_conv6.init_tensor(weights_conv6, DataType::F32);
        npy_bias_conv6   .open(bias_datapath   +bias_name[5]   ); npy_bias_conv6   .init_tensor(bias_conv6   , DataType::F32);
		// const TensorShape weights_conv6_shape(3,3,256,256);
        // weights_conv6.allocator()->init(TensorInfo(weights_conv6_shape, 1, DataType::F32));
        // const TensorShape bias_conv6_shape(256);
        // bias_conv6.allocator()->init(TensorInfo(bias_conv6_shape, 1, DataType::F32));
		const TensorShape out_shape_conv6(56, 56, 256);
		out_conv6.allocator()->init(TensorInfo(out_shape_conv6, 1, DataType:: F32));
		out_act6.allocator()->init(TensorInfo(out_shape_conv6,1,DataType:: F32));


        NPYLoader npy_weights_conv7, npy_bias_conv7;
        npy_weights_conv7.open(weights_datapath+weights_name[6]); npy_weights_conv7.init_tensor(weights_conv7, DataType::F32);
        npy_bias_conv7   .open(bias_datapath   +bias_name[6]   ); npy_bias_conv7   .init_tensor(bias_conv7   , DataType::F32);
		// const TensorShape weights_conv7_shape(3,3,256,256);
        // weights_conv7.allocator()->init(TensorInfo(weights_conv7_shape, 1, DataType::F32));
        // const TensorShape bias_conv7_shape(256);
        // bias_conv7.allocator()->init(TensorInfo(bias_conv7_shape, 1, DataType::F32));
		const TensorShape out_shape_conv7(56, 56, 256);
		out_conv7.allocator()->init(TensorInfo(out_shape_conv7, 1, DataType:: F32));
		out_act7.allocator()->init(TensorInfo(out_shape_conv7,1,DataType:: F32));
		TensorShape out_shape_pool3 = out_shape_conv7;
		out_shape_pool3.set(0, out_shape_pool3.x() / 2);
		out_shape_pool3.set(1, out_shape_pool3.y() / 2);
		out_pool3.allocator()->init(TensorInfo(out_shape_pool3, 1, DataType:: F32));      

        NPYLoader npy_weights_conv8, npy_bias_conv8;
        npy_weights_conv8.open(weights_datapath+weights_name[7]); npy_weights_conv8.init_tensor(weights_conv8, DataType::F32);
        npy_bias_conv8   .open(bias_datapath   +bias_name[7]   ); npy_bias_conv8   .init_tensor(bias_conv8   , DataType::F32);
		// const TensorShape weights_conv8_shape(3,3,256,512);
        // weights_conv8.allocator()->init(TensorInfo(weights_conv8_shape, 1, DataType::F32));
        // const TensorShape bias_conv8_shape(512);
        // bias_conv8.allocator()->init(TensorInfo(bias_conv8_shape, 1, DataType::F32));
		const TensorShape out_shape_conv8(28, 28, 512);
		out_conv8.allocator()->init(TensorInfo(out_shape_conv8, 1, DataType:: F32));
		out_act8.allocator()->init(TensorInfo(out_shape_conv8,1,DataType:: F32));

        NPYLoader npy_weights_conv9, npy_bias_conv9;
        npy_weights_conv9.open(weights_datapath+weights_name[8]); npy_weights_conv9.init_tensor(weights_conv9, DataType::F32);
        npy_bias_conv9   .open(bias_datapath   +bias_name[8]   ); npy_bias_conv9   .init_tensor(bias_conv9   , DataType::F32);
		// const TensorShape weights_conv9_shape(3,3,512,512);
        // weights_conv9.allocator()->init(TensorInfo(weights_conv9_shape, 1, DataType::F32));
        // const TensorShape bias_conv9_shape(512);
        // bias_conv9.allocator()->init(TensorInfo(bias_conv9_shape, 1, DataType::F32));
		const TensorShape out_shape_conv9(28, 28, 512);
		out_conv9.allocator()->init(TensorInfo(out_shape_conv9, 1, DataType:: F32));
		out_act9.allocator()->init(TensorInfo(out_shape_conv9,1,DataType:: F32));

        NPYLoader npy_weights_conv10, npy_bias_conv10;
        npy_weights_conv10.open(weights_datapath+weights_name[9]); npy_weights_conv10.init_tensor(weights_conv10, DataType::F32);
        npy_bias_conv10   .open(bias_datapath   +bias_name[9]   ); npy_bias_conv10   .init_tensor(bias_conv10   , DataType::F32);
		// const TensorShape weights_conv10_shape(3,3,512,512);
        // weights_conv10.allocator()->init(TensorInfo(weights_conv10_shape, 1, DataType::F32));
        // const TensorShape bias_conv10_shape(512);
        // bias_conv10.allocator()->init(TensorInfo(bias_conv10_shape, 1, DataType::F32));
		const TensorShape out_shape_conv10(28, 28, 512);
		out_conv10.allocator()->init(TensorInfo(out_shape_conv10, 1, DataType:: F32));
		out_act10.allocator()->init(TensorInfo(out_shape_conv10,1,DataType:: F32));
		TensorShape out_shape_pool4 = out_shape_conv10;
		out_shape_pool4.set(0, out_shape_pool4.x() / 2);
		out_shape_pool4.set(1, out_shape_pool4.y() / 2);
		out_pool4.allocator()->init(TensorInfo(out_shape_pool4, 1, DataType:: F32));      

        NPYLoader npy_weights_conv11, npy_bias_conv11;
        npy_weights_conv11.open(weights_datapath+weights_name[10]); npy_weights_conv11.init_tensor(weights_conv11, DataType::F32);
        npy_bias_conv11   .open(bias_datapath   +bias_name[10]   ); npy_bias_conv11   .init_tensor(bias_conv11   , DataType::F32);
		// const TensorShape weights_conv11_shape(3,3,512,512);
        // weights_conv11.allocator()->init(TensorInfo(weights_conv11_shape, 1, DataType::F32));
        // const TensorShape bias_conv11_shape(512);
        // bias_conv11.allocator()->init(TensorInfo(bias_conv11_shape, 1, DataType::F32));
		const TensorShape out_shape_conv11(14, 14, 512);
		out_conv11.allocator()->init(TensorInfo(out_shape_conv11, 1, DataType:: F32));
		out_act11.allocator()->init(TensorInfo(out_shape_conv11,1,DataType:: F32));

        NPYLoader npy_weights_conv12, npy_bias_conv12;
        npy_weights_conv12.open(weights_datapath+weights_name[11]); npy_weights_conv12.init_tensor(weights_conv12, DataType::F32);
        npy_bias_conv12   .open(bias_datapath   +bias_name[11]   ); npy_bias_conv12   .init_tensor(bias_conv12   , DataType::F32);
		// const TensorShape weights_conv12_shape(3,3,512,512);
        // weights_conv12.allocator()->init(TensorInfo(weights_conv12_shape, 1, DataType::F32));
        // const TensorShape bias_conv12_shape(512);
        // bias_conv12.allocator()->init(TensorInfo(bias_conv12_shape, 1, DataType::F32));
		const TensorShape out_shape_conv12(14, 14, 512);
		out_conv12.allocator()->init(TensorInfo(out_shape_conv12, 1, DataType:: F32));
		out_act12.allocator()->init(TensorInfo(out_shape_conv12,1,DataType:: F32));

        NPYLoader npy_weights_conv13, npy_bias_conv13;
        npy_weights_conv13.open(weights_datapath+weights_name[12]); npy_weights_conv13.init_tensor(weights_conv13, DataType::F32);
        npy_bias_conv13   .open(bias_datapath   +bias_name[12]   ); npy_bias_conv13   .init_tensor(bias_conv13   , DataType::F32);
		// const TensorShape weights_conv13_shape(3,3,512,512);
        // weights_conv13.allocator()->init(TensorInfo(weights_conv13_shape, 1, DataType::F32));
        // const TensorShape bias_conv13_shape(512);
        // bias_conv13.allocator()->init(TensorInfo(bias_conv13_shape, 1, DataType::F32));
		const TensorShape out_shape_conv13(14, 14, 512);
		out_conv13.allocator()->init(TensorInfo(out_shape_conv13, 1, DataType:: F32));
		out_act13.allocator()->init(TensorInfo(out_shape_conv13,1,DataType:: F32));
		TensorShape out_shape_pool5 = out_shape_conv13;
		out_shape_pool5.set(0, out_shape_pool5.x() / 2);
		out_shape_pool5.set(1, out_shape_pool5.y() / 2);
		out_pool5.allocator()->init(TensorInfo(out_shape_pool5, 1, DataType:: F32));      

        NPYLoader npy_weights_conv14, npy_bias_conv14;
        npy_weights_conv14.open(weights_datapath+weights_name[13]); npy_weights_conv14.init_tensor(weights_conv14, DataType::F32);
        npy_bias_conv14   .open(bias_datapath   +bias_name[13]   ); npy_bias_conv14   .init_tensor(bias_conv14   , DataType::F32);
		// const TensorShape weights_conv14_shape(out_shape_pool5.x()*out_shape_pool5.y()*out_shape_pool5.z(), 4096);
        // weights_conv14.allocator()->init(TensorInfo(weights_conv14_shape, 1, DataType::F32));
        // const TensorShape bias_conv14_shape(4096);
        // bias_conv14.allocator()->init(TensorInfo(bias_conv14_shape, 1, DataType::F32));
		const TensorShape out_shape_conv14(4096);
		out_conv14.allocator()->init(TensorInfo(out_shape_conv14, 1, DataType:: F32));
		out_act14.allocator()->init(TensorInfo(out_shape_conv14,1,DataType:: F32));

        NPYLoader npy_weights_conv15, npy_bias_conv15;
        npy_weights_conv15.open(weights_datapath+weights_name[14]); npy_weights_conv15.init_tensor(weights_conv15, DataType::F32);
        npy_bias_conv15   .open(bias_datapath   +bias_name[14]   ); npy_bias_conv15   .init_tensor(bias_conv15   , DataType::F32);
        // const TensorShape weights_conv15_shape(4096, 4096);
        // weights_conv15.allocator()->init(TensorInfo(weights_conv15_shape, 1, DataType::F32));
        // const TensorShape bias_conv15_shape(4096);
        // bias_conv15.allocator()->init(TensorInfo(bias_conv15_shape, 1, DataType::F32));
		const TensorShape out_shape_conv15(4096);
		out_conv15.allocator()->init(TensorInfo(out_shape_conv15, 1, DataType:: F32));
		out_act15.allocator()->init(TensorInfo(out_shape_conv15,1,DataType:: F32));

        NPYLoader npy_weights_conv16, npy_bias_conv16;
        npy_weights_conv16.open(weights_datapath+weights_name[15]); npy_weights_conv16.init_tensor(weights_conv16, DataType::F32);
        npy_bias_conv16   .open(bias_datapath   +bias_name[15]   ); npy_bias_conv16   .init_tensor(bias_conv16   , DataType::F32);
		// const TensorShape weights_conv16_shape(4096, 1000);
        // weights_conv16.allocator()->init(TensorInfo(weights_conv16_shape, 1, DataType::F32));
        // const TensorShape bias_conv16_shape(1000);
        // bias_conv16.allocator()->init(TensorInfo(bias_conv16_shape, 1, DataType::F32));
		const TensorShape out_shape_conv16(1000);
		out.allocator()->init(TensorInfo(out_shape_conv16, 1, DataType:: F32));

		/*configure start*/
		/*conv layers*/
		conv1.configure(&input, &weights_conv1, &bias_conv1, &out_conv1, PadStrideInfo(1,1,1,1));
		act1.configure(&out_conv1, &out_act1, ActivationLayerInfo(ActivationLayerInfo::ActivationFunction::RELU));

		conv2.configure(&out_act1, &weights_conv2, &bias_conv2, &out_conv2, PadStrideInfo(1,1,1,1));
		act2.configure(&out_conv2, &out_act2, ActivationLayerInfo(ActivationLayerInfo::ActivationFunction::RELU));
		pool1.configure(&out_act2, &out_pool1,PoolingLayerInfo(PoolingType::MAX, 2, PadStrideInfo(2, 2, 0, 0)) );

		conv3.configure(&out_pool1, &weights_conv3, &bias_conv3, &out_conv3, PadStrideInfo(1,1,1,1));
		act3.configure(&out_conv3, &out_act3, ActivationLayerInfo(ActivationLayerInfo::ActivationFunction::RELU));


		conv4.configure(&out_act3, &weights_conv4, &bias_conv4, &out_conv4, PadStrideInfo(1,1,1,1));
		act4.configure(&out_conv4, &out_act4, ActivationLayerInfo(ActivationLayerInfo::ActivationFunction::RELU));
		pool2.configure(&out_act4, &out_pool2,PoolingLayerInfo(PoolingType::MAX, 2, PadStrideInfo(2, 2, 0, 0)) );

		conv5.configure(&out_pool2, &weights_conv5, &bias_conv5, &out_conv5, PadStrideInfo(1,1,1,1));
		act5.configure(&out_conv5, &out_act5, ActivationLayerInfo(ActivationLayerInfo::ActivationFunction::RELU));

		conv6.configure(&out_act5, &weights_conv6, &bias_conv6, &out_conv6, PadStrideInfo(1,1,1,1));
		act6.configure(&out_conv6, &out_act6, ActivationLayerInfo(ActivationLayerInfo::ActivationFunction::RELU));

		conv7.configure(&out_act6, &weights_conv7, &bias_conv7, &out_conv7, PadStrideInfo(1,1,1,1));
		act7.configure(&out_conv7, &out_act7, ActivationLayerInfo(ActivationLayerInfo::ActivationFunction::RELU));
		pool3.configure(&out_act7, &out_pool3,PoolingLayerInfo(PoolingType::MAX, 2, PadStrideInfo(2, 2, 0, 0)) );

		conv8.configure(&out_pool3, &weights_conv8, &bias_conv8, &out_conv8, PadStrideInfo(1,1,1,1));
		act8.configure(&out_conv8, &out_act8, ActivationLayerInfo(ActivationLayerInfo::ActivationFunction::RELU));

		conv9.configure(&out_act8, &weights_conv9, &bias_conv9, &out_conv9, PadStrideInfo(1,1,1,1));
		act9.configure(&out_conv9, &out_act9, ActivationLayerInfo(ActivationLayerInfo::ActivationFunction::RELU));

		conv10.configure(&out_act9, &weights_conv10, &bias_conv10, &out_conv10, PadStrideInfo(1,1,1,1));
		act10.configure(&out_conv10, &out_act10, ActivationLayerInfo(ActivationLayerInfo::ActivationFunction::RELU));
		pool4.configure(&out_act10, &out_pool4,PoolingLayerInfo(PoolingType::MAX, 2, PadStrideInfo(2, 2, 0, 0)) );

		conv11.configure(&out_pool4, &weights_conv11, &bias_conv11, &out_conv11, PadStrideInfo(1,1,1,1));
		act11.configure(&out_conv11, &out_act11, ActivationLayerInfo(ActivationLayerInfo::ActivationFunction::RELU));

		conv12.configure(&out_act11, &weights_conv12, &bias_conv12, &out_conv12, PadStrideInfo(1,1,1,1));
		act12.configure(&out_conv12, &out_act12, ActivationLayerInfo(ActivationLayerInfo::ActivationFunction::RELU));

		conv13.configure(&out_act12, &weights_conv13, &bias_conv13, &out_conv13, PadStrideInfo(1,1,1,1));
		act13.configure(&out_conv13, &out_act13, ActivationLayerInfo(ActivationLayerInfo::ActivationFunction::RELU));
		pool5.configure(&out_act13, &out_pool5,PoolingLayerInfo(PoolingType::MAX, 2, PadStrideInfo(2, 2, 0, 0)) );

        
		conv14.configure(&out_pool5, &weights_conv14, &bias_conv14, &out_conv14);
		act14.configure(&out_conv14, &out_act14, ActivationLayerInfo(ActivationLayerInfo::ActivationFunction::RELU));

		conv15.configure(&out_act14, &weights_conv15, &bias_conv15, &out_conv15);
		act15.configure(&out_conv15, &out_act15, ActivationLayerInfo(ActivationLayerInfo::ActivationFunction::RELU));

		conv16.configure(&out_act15, &weights_conv16, &bias_conv16, &out);


		/*allocate start*/
		input.allocator()->allocate(); 

		weights_conv1.allocator()->allocate();  bias_conv1.allocator()->allocate();
		weights_conv2.allocator()->allocate();  bias_conv2.allocator()->allocate();
		weights_conv3.allocator()->allocate();  bias_conv3.allocator()->allocate();
		weights_conv4.allocator()->allocate();  bias_conv4.allocator()->allocate();
		weights_conv5.allocator()->allocate();  bias_conv5.allocator()->allocate();
		weights_conv6.allocator()->allocate();  bias_conv6.allocator()->allocate();
		weights_conv7.allocator()->allocate();  bias_conv7.allocator()->allocate();
		weights_conv8.allocator()->allocate();  bias_conv8.allocator()->allocate();
		weights_conv9.allocator()->allocate();  bias_conv9.allocator()->allocate();
		weights_conv10.allocator()->allocate(); bias_conv10.allocator()->allocate();
		weights_conv11.allocator()->allocate(); bias_conv11.allocator()->allocate();
		weights_conv12.allocator()->allocate(); bias_conv12.allocator()->allocate();
		weights_conv13.allocator()->allocate(); bias_conv13.allocator()->allocate();
		weights_conv14.allocator()->allocate(); bias_conv14.allocator()->allocate();
		weights_conv15.allocator()->allocate(); bias_conv15.allocator()->allocate();
		weights_conv16.allocator()->allocate(); bias_conv16.allocator()->allocate();

		out_conv1.allocator()->allocate(); out_conv2.allocator()->allocate(); out_conv3.allocator()->allocate(); out_conv4.allocator()->allocate();
		out_conv5.allocator()->allocate(); out_conv6.allocator()->allocate(); out_conv7.allocator()->allocate(); out_conv8.allocator()->allocate();
		out_conv9.allocator()->allocate(); out_conv10.allocator()->allocate(); out_conv11.allocator()->allocate(); out_conv12.allocator()->allocate();
		out_conv13.allocator()->allocate(); out_conv14.allocator()->allocate(); out_conv15.allocator()->allocate(); out.allocator()->allocate();

		out_act1.allocator()->allocate(); out_act2.allocator()->allocate(); out_act3.allocator()->allocate(); out_act4.allocator()->allocate(); out_act5.allocator()->allocate();
		out_act6.allocator()->allocate(); out_act7.allocator()->allocate(); out_act8.allocator()->allocate(); out_act9.allocator()->allocate(); out_act10.allocator()->allocate();
		out_act11.allocator()->allocate(); out_act12.allocator()->allocate(); out_act13.allocator()->allocate(); out_act14.allocator()->allocate(); out_act15.allocator()->allocate();

		out_pool1.allocator()->allocate(); out_pool2.allocator()->allocate(); out_pool3.allocator()->allocate(); out_pool4.allocator()->allocate(); out_pool5.allocator()->allocate();

        /*FILL TENSOR*/
        npy_input.fill_tensor(input);
        npy_weights_conv1.fill_tensor(weights_conv1); npy_bias_conv1.fill_tensor(bias_conv1);
        npy_weights_conv2.fill_tensor(weights_conv2); npy_bias_conv2.fill_tensor(bias_conv2);
        npy_weights_conv3.fill_tensor(weights_conv3); npy_bias_conv3.fill_tensor(bias_conv3);
        npy_weights_conv4.fill_tensor(weights_conv4); npy_bias_conv4.fill_tensor(bias_conv4);
        npy_weights_conv5.fill_tensor(weights_conv5); npy_bias_conv5.fill_tensor(bias_conv5);
        npy_weights_conv6.fill_tensor(weights_conv6); npy_bias_conv6.fill_tensor(bias_conv6);
        npy_weights_conv7.fill_tensor(weights_conv7); npy_bias_conv7.fill_tensor(bias_conv7);
        npy_weights_conv8.fill_tensor(weights_conv8); npy_bias_conv8.fill_tensor(bias_conv8);
        npy_weights_conv9.fill_tensor(weights_conv9); npy_bias_conv9.fill_tensor(bias_conv9);
        npy_weights_conv10.fill_tensor(weights_conv10); npy_bias_conv10.fill_tensor(bias_conv10);
        npy_weights_conv11.fill_tensor(weights_conv11); npy_bias_conv11.fill_tensor(bias_conv11);
        npy_weights_conv12.fill_tensor(weights_conv12); npy_bias_conv12.fill_tensor(bias_conv12);
        npy_weights_conv13.fill_tensor(weights_conv13); npy_bias_conv13.fill_tensor(bias_conv13);
        npy_weights_conv14.fill_tensor(weights_conv14); npy_bias_conv14.fill_tensor(bias_conv14);
        npy_weights_conv15.fill_tensor(weights_conv15); npy_bias_conv15.fill_tensor(bias_conv15);
        npy_weights_conv16.fill_tensor(weights_conv16); npy_bias_conv16.fill_tensor(bias_conv16);
		/*allocate end*/
		return true;
}/*end of do_setup*/
void do_run()override
{
    double conv_layer=0, act_layer=0, fc_layer=0, norm_layer=0, pool_layer=0, other_layer=0;
	double lend_conv1=0, lend_act1=0;
	double lend_conv2=0, lend_act2=0, lend_pool1=0;
	double lend_conv3=0, lend_act3=0;
	double lend_conv4=0, lend_act4=0, lend_pool2=0;
	double lend_conv5=0, lend_act5=0;
	double lend_conv6=0, lend_act6=0;
	double lend_conv7=0, lend_act7=0, lend_pool3=0;
	double lend_conv8=0, lend_act8=0;
	double lend_conv9=0, lend_act9=0;
	double lend_conv10=0, lend_act10=0, lend_pool4=0;
	double lend_conv11=0, lend_act11=0;
	double lend_conv12=0, lend_act12=0;
	double lend_conv13=0, lend_act13=0, lend_pool5=0;
	double lend_conv14=0, lend_act14=0;
	double lend_conv15=0, lend_act15=0;
	double lend_conv16=0;

	double total_time=0, time=0;
    unsigned int cycles=101;
	for (unsigned int i = 0; i < cycles; i++)
	{
		auto start = std::chrono::high_resolution_clock::now();

		conv1.run(); auto end1_1=std::chrono::high_resolution_clock::now();
		act1.run(); auto end1_2=std::chrono::high_resolution_clock::now();

		conv2.run();auto end2_1=std::chrono::high_resolution_clock::now();
		act2.run();auto end2_2=std::chrono::high_resolution_clock::now();
		pool1.run();auto end2_3=std::chrono::high_resolution_clock::now();

		conv3.run();auto end3_1=std::chrono::high_resolution_clock::now();
		act3.run();auto end3_2=std::chrono::high_resolution_clock::now();

		conv4.run();auto end4_1=std::chrono::high_resolution_clock::now();
		act4.run();auto end4_2=std::chrono::high_resolution_clock::now();
		pool2.run();auto end4_3=std::chrono::high_resolution_clock::now();

		conv5.run();auto end5_1=std::chrono::high_resolution_clock::now();
		act5.run();auto end5_2=std::chrono::high_resolution_clock::now();

		conv6.run();auto end6_1=std::chrono::high_resolution_clock::now();
		act6.run();auto end6_2=std::chrono::high_resolution_clock::now();

		conv7.run();auto end7_1=std::chrono::high_resolution_clock::now();
		act7.run();auto end7_2=std::chrono::high_resolution_clock::now();
		pool3.run();auto end7_3=std::chrono::high_resolution_clock::now();

		conv8.run();auto end8_1=std::chrono::high_resolution_clock::now();
		act8.run();auto end8_2=std::chrono::high_resolution_clock::now();

		conv9.run();auto end9_1=std::chrono::high_resolution_clock::now();
		act9.run();auto end9_2=std::chrono::high_resolution_clock::now();

		conv10.run();auto end10_1=std::chrono::high_resolution_clock::now();
		act10.run();auto end10_2=std::chrono::high_resolution_clock::now();
		pool4.run();auto end10_3=std::chrono::high_resolution_clock::now();

		conv11.run();auto end11_1=std::chrono::high_resolution_clock::now();
		act11.run();auto end11_2=std::chrono::high_resolution_clock::now();

		conv12.run();auto end12_1=std::chrono::high_resolution_clock::now();
		act12.run();auto end12_2=std::chrono::high_resolution_clock::now();

		conv13.run();auto end13_1=std::chrono::high_resolution_clock::now();
		act13.run();auto end13_2=std::chrono::high_resolution_clock::now();
		pool5.run();auto end13_3=std::chrono::high_resolution_clock::now();

		conv14.run();auto end14_1=std::chrono::high_resolution_clock::now();
		act14.run();auto end14_2=std::chrono::high_resolution_clock::now();

		conv15.run();auto end15_1=std::chrono::high_resolution_clock::now();
		act15.run();auto end15_2=std::chrono::high_resolution_clock::now();

		conv16.run();auto end16_1=std::chrono::high_resolution_clock::now();
		
		/* calculate layer inference time*/
		if(i>0){
			double one_runtime=0;

			time = std::chrono::duration_cast<std::chrono::duration<double>>(end1_1 - start).count();lend_conv1+=time; one_runtime+=time; conv_layer+=time;
			time = std::chrono::duration_cast<std::chrono::duration<double>>(end1_2 - end1_1).count();lend_act1+=time; one_runtime+=time; act_layer +=time;

			time = std::chrono::duration_cast<std::chrono::duration<double>>(end2_1 - end1_2).count();lend_conv2+=time; one_runtime+=time; conv_layer+=time;
			time = std::chrono::duration_cast<std::chrono::duration<double>>(end2_2 - end2_1).count();lend_act2+=time; one_runtime+=time; act_layer +=time;
			time = std::chrono::duration_cast<std::chrono::duration<double>>(end2_3 - end2_2).count();lend_pool1+=time; one_runtime+=time; pool_layer+=time;

			time = std::chrono::duration_cast<std::chrono::duration<double>>(end3_1 - end2_3).count();lend_conv3+=time; one_runtime+=time; conv_layer+=time;
			time = std::chrono::duration_cast<std::chrono::duration<double>>(end3_2 - end3_1).count();lend_act3+=time; one_runtime+=time; act_layer +=time;

			time = std::chrono::duration_cast<std::chrono::duration<double>>(end4_1 - end3_2).count();lend_conv4+=time; one_runtime+=time; conv_layer+=time;
			time = std::chrono::duration_cast<std::chrono::duration<double>>(end4_2 - end4_1).count();lend_act4+=time; one_runtime+=time; act_layer +=time;
			time = std::chrono::duration_cast<std::chrono::duration<double>>(end4_3 - end4_2).count();lend_pool2+=time; one_runtime+=time; pool_layer+=time;

			time = std::chrono::duration_cast<std::chrono::duration<double>>(end5_1 - end4_3).count();lend_conv5+=time; one_runtime+=time; conv_layer+=time;
			time = std::chrono::duration_cast<std::chrono::duration<double>>(end5_2 - end5_1).count();lend_act5+=time; one_runtime+=time; act_layer +=time;

			time = std::chrono::duration_cast<std::chrono::duration<double>>(end6_1 - end5_2).count();lend_conv6+=time; one_runtime+=time; conv_layer+=time;
			time = std::chrono::duration_cast<std::chrono::duration<double>>(end6_2 - end6_1).count();lend_act6+=time; one_runtime+=time; act_layer +=time;

			time = std::chrono::duration_cast<std::chrono::duration<double>>(end7_1 - end6_2).count();lend_conv7+=time; one_runtime+=time; conv_layer+=time;
			time = std::chrono::duration_cast<std::chrono::duration<double>>(end7_2 - end7_1).count();lend_act7+=time; one_runtime+=time; act_layer +=time;
			time = std::chrono::duration_cast<std::chrono::duration<double>>(end7_3 - end7_2).count();lend_pool3+=time; one_runtime+=time; pool_layer+=time;

			time = std::chrono::duration_cast<std::chrono::duration<double>>(end8_1 - end7_3).count();lend_conv8+=time; one_runtime+=time; conv_layer+=time;
			time = std::chrono::duration_cast<std::chrono::duration<double>>(end8_2 - end8_1).count();lend_act8+=time; one_runtime+=time; act_layer +=time;

			time = std::chrono::duration_cast<std::chrono::duration<double>>(end9_1 - end8_2).count();lend_conv9+=time; one_runtime+=time; conv_layer+=time;
			time = std::chrono::duration_cast<std::chrono::duration<double>>(end9_2 - end9_1).count();lend_act9+=time; one_runtime+=time; act_layer +=time;

			time = std::chrono::duration_cast<std::chrono::duration<double>>(end10_1 - end9_2).count();lend_conv10+=time; one_runtime+=time; conv_layer+=time;
			time = std::chrono::duration_cast<std::chrono::duration<double>>(end10_2 - end10_1).count();lend_act10+=time; one_runtime+=time; act_layer +=time;
			time = std::chrono::duration_cast<std::chrono::duration<double>>(end10_3 - end10_2).count();lend_pool4+=time; one_runtime+=time; pool_layer+=time;

			time = std::chrono::duration_cast<std::chrono::duration<double>>(end11_1 - end10_3).count();lend_conv11+=time; one_runtime+=time; conv_layer+=time;
			time = std::chrono::duration_cast<std::chrono::duration<double>>(end11_2 - end11_1).count();lend_act11+=time; one_runtime+=time; act_layer +=time;

			time = std::chrono::duration_cast<std::chrono::duration<double>>(end12_1 - end11_2).count();lend_conv12+=time; one_runtime+=time; conv_layer+=time;
			time = std::chrono::duration_cast<std::chrono::duration<double>>(end12_2 - end12_1).count();lend_act12+=time; one_runtime+=time; act_layer +=time;

			time = std::chrono::duration_cast<std::chrono::duration<double>>(end13_1 - end12_2).count();lend_conv13+=time; one_runtime+=time; conv_layer+=time;
			time = std::chrono::duration_cast<std::chrono::duration<double>>(end13_2 - end13_1).count();lend_act13+=time; one_runtime+=time; act_layer +=time;
			time = std::chrono::duration_cast<std::chrono::duration<double>>(end13_3 - end13_2).count();lend_pool5+=time; one_runtime+=time; pool_layer+=time;

			time = std::chrono::duration_cast<std::chrono::duration<double>>(end14_1 - end13_3).count();lend_conv14+=time; one_runtime+=time; fc_layer+=time;
			time = std::chrono::duration_cast<std::chrono::duration<double>>(end14_2 - end14_1).count();lend_act14+=time; one_runtime+=time; act_layer +=time;

			time = std::chrono::duration_cast<std::chrono::duration<double>>(end15_1 - end14_2).count();lend_conv15+=time; one_runtime+=time; fc_layer+=time;
			time = std::chrono::duration_cast<std::chrono::duration<double>>(end15_2 - end15_1).count();lend_act15+=time; one_runtime+=time; act_layer +=time;

			time = std::chrono::duration_cast<std::chrono::duration<double>>(end16_1 - end15_2).count();lend_conv16+=time; one_runtime+=time; fc_layer+=time;
			
			if(i>0){
                std::cout<<i<<"---run:"<<std::endl;
                std::cout<<"time="<<one_runtime*1000<<"ms"<<std::endl;
                total_time+=one_runtime;    }
			if(i==0){
				std::cout<<"First run: "<<std::endl;
				std::cout<<"time="<<one_runtime*1000<<"ms"<<std::endl;
			}
		}
	}
		
		/*print time*/
		std::cout<<"VGG16"<<std::endl;
		std::cout << "---conv1       " << "		"<< lend_conv1 * 1000/cycles << "ms" << std::endl;
		std::cout << "---relu1       " <<"		"<< lend_act1* 1000/cycles << "ms" << std::endl;

		std::cout << "---conv2       " << "		"<< lend_conv2 * 1000/cycles << "ms" << std::endl;
		std::cout << "---relu2       " <<"		"<< lend_act2* 1000/cycles << "ms" << std::endl;
		std::cout << "---pooling    " <<"		"<< lend_pool1 * 1000/cycles << "ms" << std::endl;

		std::cout << "---conv3       " << "		"<< lend_conv3 * 1000/cycles << "ms" << std::endl;
		std::cout << "---relu3       " <<"		"<< lend_act3* 1000/cycles << "ms" << std::endl;

		std::cout << "---conv4       " << "		"<< lend_conv4 * 1000/cycles << "ms" << std::endl;
		std::cout << "---relu4       " <<"		"<< lend_act4* 1000/cycles << "ms" << std::endl;
		std::cout << "---pooling    " <<"		"<< lend_pool2 * 1000/cycles << "ms" << std::endl;

		std::cout << "---conv5       " << "		"<< lend_conv5 * 1000/cycles << "ms" << std::endl;
		std::cout << "---relu5       " <<"		"<< lend_act5* 1000/cycles << "ms" << std::endl;

		std::cout << "---conv6       " << "		"<< lend_conv6 * 1000/cycles << "ms" << std::endl;
		std::cout << "---relu6       " <<"		"<< lend_act6* 1000/cycles << "ms" << std::endl;

		std::cout << "---conv7       " << "		"<< lend_conv7 * 1000/cycles << "ms" << std::endl;
		std::cout << "---relu7       " <<"		"<< lend_act7* 1000/cycles << "ms" << std::endl;
		std::cout << "---pooling    " <<"		"<< lend_pool3 *  1000/cycles << "ms" << std::endl;

		std::cout << "---conv8       " << "		"<< lend_conv8 * 1000/cycles << "ms" << std::endl;
		std::cout << "---relu8       " <<"		"<< lend_act8* 1000/cycles << "ms" << std::endl;

		std::cout << "---conv9       " << "		"<< lend_conv9 * 1000/cycles << "ms" << std::endl;
		std::cout << "---relu9       " <<"		"<< lend_act9* 1000/cycles << "ms" << std::endl;

		std::cout << "---conv10       " << "		"<< lend_conv10 * 1000/cycles << "ms" << std::endl;
		std::cout << "---relu10       " <<"		"<< lend_act10* 1000/cycles << "ms" << std::endl;
		std::cout << "---pooling    " <<"		"<< lend_pool4 * 1000/cycles << "ms" << std::endl;

		std::cout << "---conv11       " << "		"<< lend_conv11 * 1000/cycles << "ms" << std::endl;
		std::cout << "---relu11       " <<"		"<< lend_act11* 1000/cycles << "ms" << std::endl;

		std::cout << "---conv12       " << "		"<< lend_conv12 * 1000/cycles << "ms" << std::endl;
		std::cout << "---relu12       " <<"		"<< lend_act12* 1000/cycles << "ms" << std::endl;

		std::cout << "---conv13       " << "		"<< lend_conv13 * 1000/cycles << "ms" << std::endl;
		std::cout << "---relu13       " <<"		"<< lend_act13* 1000/cycles << "ms" << std::endl;
		std::cout << "---pooling    " <<"		"<< lend_pool5 * 1000/cycles << "ms" << std::endl;

		std::cout << "---fc1          " << "		"<< lend_conv14 * 1000/cycles << "ms" << std::endl;
		std::cout << "---relu14       " <<"		"<< lend_act14* 1000/cycles << "ms" << std::endl;

		std::cout << "---fc2          " << "		"<< lend_conv15 * 1000/cycles << "ms" << std::endl;
		std::cout << "---relu15       " <<"		"<< lend_act15* 1000/cycles << "ms" << std::endl;

		std::cout << "---fc3          " << "		"<< lend_conv16 * 1000/cycles << "ms" << std::endl;
	
		std::cout<<"avg time="<<total_time*1000/(cycles-1)<<"ms"<<std::endl;
        std::cout<<"conv layers: "<<conv_layer*1000/(cycles-1)<<"ms"<<std::endl;
        std::cout<<"act  layers: "<<act_layer*1000/(cycles-1) <<"ms"<<std::endl;
        std::cout<<"pool layers: "<<pool_layer*1000/(cycles-1)<<"ms"<<std::endl;
        std::cout<<"norm layers: "<<norm_layer*1000/(cycles-1)<<"ms"<<std::endl;
        std::cout<<"fc   layers: "<<fc_layer*1000/(cycles-1)  <<"ms"<<std::endl;
        std::cout<<"other layers: "<<other_layer*1000/(cycles-1)<<"ms"<<std::endl;

}
private:
    /*datapath*/
    string weights_datapath="/media/sdcard/ComputeLibrary/data/neon_vgg_float/weights/";
    string bias_datapath   ="/media/sdcard/ComputeLibrary/data/neon_vgg_float/bias/";
    string weights_name[16]={
        "features.0.weight.npy",
        "features.2.weight.npy",
        "features.5.weight.npy",
        "features.7.weight.npy",
        "features.10.weight.npy",
        "features.12.weight.npy",
        "features.14.weight.npy",
        "features.17.weight.npy",
        "features.19.weight.npy",
        "features.21.weight.npy",
        "features.24.weight.npy",
        "features.26.weight.npy",
        "features.28.weight.npy",
        "classifier.0.weight.npy",
        "classifier.3.weight.npy",
        "classifier.6.weight.npy"
    };
    string bias_name[16]={
        "features.0.bias.npy",
        "features.2.bias.npy",
        "features.5.bias.npy",
        "features.7.bias.npy",
        "features.10.bias.npy",
        "features.12.bias.npy",
        "features.14.bias.npy",
        "features.17.bias.npy",
        "features.19.bias.npy",
        "features.21.bias.npy",
        "features.24.bias.npy",
        "features.26.bias.npy",
        "features.28.bias.npy",
        "classifier.0.bias.npy",
        "classifier.3.bias.npy",
        "classifier.6.bias.npy"
    };
	/*Tensor*/
		Tensor input{}; 

		Tensor weights_conv1{};  Tensor bias_conv1{};
		Tensor weights_conv2{};  Tensor bias_conv2{};
		Tensor weights_conv3{};  Tensor bias_conv3{};
		Tensor weights_conv4{};  Tensor bias_conv4{};
		Tensor weights_conv5{};  Tensor bias_conv5{};
		Tensor weights_conv6{};  Tensor bias_conv6{};
		Tensor weights_conv7{};  Tensor bias_conv7{};
		Tensor weights_conv8{};  Tensor bias_conv8{};
		Tensor weights_conv9{};  Tensor bias_conv9{};
		Tensor weights_conv10{}; Tensor bias_conv10{};
		Tensor weights_conv11{}; Tensor bias_conv11{};
		Tensor weights_conv12{}; Tensor bias_conv12{};
		Tensor weights_conv13{}; Tensor bias_conv13{};
		Tensor weights_conv14{}; Tensor bias_conv14{};
		Tensor weights_conv15{}; Tensor bias_conv15{};
		Tensor weights_conv16{}; Tensor bias_conv16{};

		Tensor out_conv1{}; Tensor out_conv2{}; Tensor out_conv3{}; Tensor out_conv4{};
		Tensor out_conv5{}; Tensor out_conv6{}; Tensor out_conv7{}; Tensor out_conv8{};
		Tensor out_conv9{}; Tensor out_conv10{}; Tensor out_conv11{}; Tensor out_conv12{};
		Tensor out_conv13{}; Tensor out_conv14{}; Tensor out_conv15{}; Tensor out{};

		Tensor out_act1{}; Tensor out_act2{}; Tensor out_act3{}; Tensor out_act4{}; Tensor out_act5{};
		Tensor out_act6{}; Tensor out_act7{}; Tensor out_act8{}; Tensor out_act9{}; Tensor out_act10{};
		Tensor out_act11{}; Tensor out_act12{}; Tensor out_act13{}; Tensor out_act14{}; Tensor out_act15{};

		Tensor out_pool1{}; Tensor out_pool2{}; Tensor out_pool3{}; Tensor out_pool4{}; Tensor out_pool5{};

		

		/*Layer */
		NEGEMMConvolutionLayer conv1{}; NEGEMMConvolutionLayer conv2{}; NEGEMMConvolutionLayer conv3{}; NEGEMMConvolutionLayer conv4{};
		NEGEMMConvolutionLayer conv5{}; NEGEMMConvolutionLayer conv6{}; NEGEMMConvolutionLayer conv7{}; NEGEMMConvolutionLayer conv8{};
		NEGEMMConvolutionLayer conv9{}; NEGEMMConvolutionLayer conv10{}; NEGEMMConvolutionLayer conv11{}; NEGEMMConvolutionLayer conv12{};
		NEGEMMConvolutionLayer conv13{}; NEFullyConnectedLayer conv14{}; NEFullyConnectedLayer conv15{}; NEFullyConnectedLayer conv16{};

		NEActivationLayer act1{}; NEActivationLayer act2{}; NEActivationLayer act3{}; NEActivationLayer act4{}; NEActivationLayer act5{};
		NEActivationLayer act6{}; NEActivationLayer act7{}; NEActivationLayer act8{}; NEActivationLayer act9{}; NEActivationLayer act10{};
		NEActivationLayer act11{}; NEActivationLayer act12{}; NEActivationLayer act13{}; NEActivationLayer act14{}; NEActivationLayer act15{};

		NEPoolingLayer pool1{}; NEPoolingLayer pool2{}; NEPoolingLayer pool3{}; NEPoolingLayer pool4{}; NEPoolingLayer pool5{};

		ConvertPolicy A{};

};/*end of class*/
int main(int argc, char **argv)
{
	return utils::run_example<NEONVGG16Example>(argc, argv);
}

