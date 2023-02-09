import onnx
import onnxruntime as ort
import numpy as np



def get_onnx_out():
    onnx_model = onnx.load("E:/BaiduSyncdisk/Project/005-卷扬乱绳识别/WindingMessRope_holy/support/vanMess/van230209-sim_320x160.onnx")
    onnx.checker.check_model(onnx_model)

    x = np.zeros((1, 3, 320, 160)).astype("float32") + 0.5
    print("input =", x)
    ort_sess = ort.InferenceSession("E:/BaiduSyncdisk/Project/005-卷扬乱绳识别/WindingMessRope_holy/support/vanMess/van230209-sim_320x160.onnx")
    outputs = ort_sess.run(["y"], {'x': x})
    print("outputs =", outputs)



if __name__ == '__main__':
    get_onnx_out()
