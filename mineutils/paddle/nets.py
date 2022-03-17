import paddle
import paddle.nn as nn
from paddle.vision.models import resnet50, resnet18
from paddle.vision.models import MobileNetV2 as MNV2
from paddlex.cls import MobileNetV3_large, MobileNetV3_small, MobileNetV3_large_ssld, MobileNetV3_small_ssld
from mineutils.colorstr import ColorStr


class ResNet50(nn.Layer):
    def __init__(self, num_classes=1000, pretrain=False):
        super(ResNet50, self).__init__()
        self.net = resnet50(pretrained=pretrain)
        self.net.fc = nn.Linear(512 * 4, num_classes)
        self.final_active = nn.Softmax()
    
    def forward(self, x):
        logits = self.net(x)
        preds = self.final_active(logits)
        return logits, preds


class ResNet18(nn.Layer):
    def __init__(self, num_classes=1000, pretrain=False):
        super(ResNet18, self).__init__()
        self.net = resnet18(pretrained=pretrain)
        self.net.fc = nn.Linear(512 * 1, num_classes)
        self.final_active = nn.Softmax()
    
    def forward(self, x):
        logits = self.net(x)
        preds = self.final_active(logits)
        return logits, preds
    
    
class MobileNetV2(nn.Layer):
    def __init__(self, num_classes=1000, scale=1.0, with_pool=True):
        super(MobileNetV2, self).__init__()
        self.net = MNV2(scale, num_classes)
        self.final_active = nn.Softmax()
        
    def forward(self, x):
        logits = self.net(x)
        preds = self.final_active(logits)
        return logits, preds
        

class MobileNetV3Small(nn.Layer):
    def __init__(self, num_classes, scale=1.0):
        super(MobileNetV3Small, self).__init__()
        model = MobileNetV3_small(num_classes, scale)
        self.net = model.net
        self.final_active = nn.Softmax()
        
        
    def forward(self, x):
        logits = self.net(x)
        preds = self.final_active(logits)
        return logits, preds


class MobileNetV3Large(nn.Layer):
    def __init__(self, num_classes, scale=1.0):
        super(MobileNetV3Large, self).__init__()
        model = MobileNetV3_large(num_classes, scale)
        self.net = model.net
        self.final_active = nn.Softmax()
    
    def forward(self, x):
        logits = self.net(x)
        preds = self.final_active(logits)
        return logits, preds
    

class MobileNetV3LargeSSLD(nn.Layer):
    def __init__(self, num_classes):
        super(MobileNetV3LargeSSLD, self).__init__()
        model = MobileNetV3_large_ssld(num_classes)
        self.net = model.net
        self.final_active = nn.Softmax()
    
    def forward(self, x):
        logits = self.net(x)
        preds = self.final_active(logits)
        return logits, preds
        
        
def getNet(name: str, init: dict, work_type="eval"):
    print("\n", init)
    net = eval(name)(**init)
    if work_type == "train":
        net.train()
    elif work_type == "eval":
        net.eval()
    else:
        print(f"{ColorStr.yellow('getNet():')} {ColorStr.fuchsia('work_type')}参数有误，因此未调整网络工作状态。")
    return net


model_dict = {"ResNet50": ResNet50, "ResNet18": ResNet18,
              "MobileNetV2": MobileNetV2, "MobileNetV3Small": MobileNetV3Small, "MobileNetV3Large": MobileNetV3Large,
              "MobileNetV3LargeSSLD": MobileNetV3LargeSSLD}


if __name__ == "__main__":
    from mineutils import useProjDir
    useProjDir()
    net = getNet("ResNet50")
    print(net)





