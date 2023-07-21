import time

import pmt
import pypmt


@pmt.measure("nvml")
def my_kernel1():
    time.sleep(1)


def my_kernel2():
    platform = "nvml"
    pm = pmt.get_pmt(platform, 0)
    start = pm.read()
    time.sleep(1)
    end = pm.read()
    return {
        "platform": platform,
        "joules": format(pypmt.PMT.joules(start, end), ".3f"),
        "seconds": format(pypmt.PMT.seconds(start, end), ".3f"),
        "watt": format(pypmt.PMT.watts(start, end), ".3f"),
    }


if __name__ == "__main__":
    print(my_kernel1())
    print(my_kernel2())
