from mineutils.colorstr import ColorStr as cs


class SingleValueMonitor:
    """
    --监测部分数据，返回bool值。
    --保证一个对象只检测一种值。
    """
    def __init__(self):
        self.values = []
        self.lowest = float("inf")
        self.highest = float("-inf")
        self.bad_existDecline_num = 0
        self.bad_existRaise_num = 0
        
    def append(self, v):
        self.values.append(v)

    def isDeclining(self, length):
        length = min(len(self.values), length)
        values = self.values[-length:]
        is_declining = all(x < y for x, y in zip(values, values[1:]))
        return is_declining

    def isRaising(self, length):
        length = min(len(self.values), length)
        values = self.values[-length:]
        is_raising = all(x > y for x, y in zip(values, values[1:]))
        return is_raising
    
    def existDecline(self, patience, show_message=False,
                     name_now_value="now_value", name_lowest="self.lowest", name_bad_existDecline_num="not_exist_decline_num"):
        now_value = self.values[-1]
        if show_message:
            print(cs.blue("SingleValueMonitor.existDecline():"),
                  cs.fuchsia(f"{name_now_value}"), f"为{now_value}",
                  cs.fuchsia(f"{name_lowest}"), f"为{self.lowest}")
        if now_value < self.lowest:
            self.lowest = now_value
            self.bad_existDecline_num = 0
        else:
            self.bad_existDecline_num += 1
            
        if self.bad_existDecline_num >= patience:
            if_exist = False
            self.bad_existDecline_num = 0
        else:
            if_exist = True
        if show_message:
            print(cs.blue("SingleValueMonitor.existDecline():"),
                  cs.fuchsia(f"{name_bad_existDecline_num}"), f"现在变为{self.bad_existDecline_num}")
        return if_exist
        
    def existRaise(self, patience, show_bad_num=False):
        now_value = self.values[-1]
        if now_value > self.highest:
            self.highest = now_value
            self.bad_existRaise_num = 0
        else:
            self.bad_existRaise_num += 1
        
        if self.bad_existRaise_num >= patience:
            if_exist = False
            self.bad_existRaise_num = 0
        else:
            if_exist = True
        if show_bad_num:
            print(cs.blue("SingleValueMonitor.existRaise():"),
                  cs.fuchsia("self.bad_existRaise_num"), f"现在变为{self.bad_existRaise_num}")
        return if_exist
    
    
if __name__ == "__main__":
    pass










