import onnx
import onnxruntime as ort
import numpy as np
import cv2



def get_onnx_out():
    model_path = r"E:\BaiduSyncdisk\Project\001-塔机辅助作业\AI-Camera\rv1106_NPU_python\models\yolov7tiny_221111_rv1106.onnx"
    image_path = "E://00500.jpg"
    onnx_model = onnx.load(model_path)
    onnx.checker.check_model(onnx_model)

    x = cv2.imread(image_path)
    x = cv2.cvtColor(x, cv2.COLOR_BGR2RGB)
    # print(x[320:322, 325:330, 0])
    x = x / 255.0
    x = x.astype("float32")
    print(x[320:322, 325:330, 0])
    x = np.expand_dims(x, axis=0)
    x = np.transpose(x, (0, 3, 1, 2))
    # print("input =", x)
    print("x.shape =", x.shape)
    ort_sess = ort.InferenceSession(model_path)
    outputs = ort_sess.run(["output", "266", "267"], {'images': x})
    out0 = outputs[0]
    out_line = np.reshape(out0, (-1))
    print("out0[0, 0:21, 1, 1] =", out0[0, 0:21, 1, 1])
    # print("out_line =", out_line[25681])
    print("out0.shape =", out0.shape)
    print("out type=", out_line.dtype)
    
    for out_data in outputs:
        name = f"E:\\VSProject\\997-examples\\rknn_example\\onnx_res_21{out_data.shape[2]}{out_data.shape[3]}.txt"
        with open(name, "wb") as f:
            f.write(out_data)
            print(f"Write {name} done")
        


if __name__ == '__main__':
    get_onnx_out()
