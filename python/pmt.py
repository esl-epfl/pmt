import pypmt
import os
import re


def is_tty_device(device_id):
    return isinstance(device_id, str) and bool(re.search(r"^/dev/tty.*", device_id))


def get_pmt(platform, device_id=0):
    if platform == "cray":
        return pypmt.Cray.create()
    elif platform == 'powersensor2':
        try:
            if is_tty_device(device_id):
                return pypmt.PowerSensor2.create(str(device_id))
            else:
                return pypmt.PowerSensor2.create()
        except AttributeError:
            raise Exception("PowerSensor2 not installed")
    elif platform == 'powersensor3':
        try:
            if is_tty_device(device_id):
                return pypmt.PowerSensor3.create(str(device_id))
            else:
                return pypmt.PowerSensor3.create()
        except AttributeError:
            raise Exception("PowerSensor3 not installed")
    elif platform == "dummy":
        return pypmt.Dummy.create()
    elif platform == "tegra":
        return pypmt.tegra.create()
    elif platform == "nvml":
        try:
            return pypmt.NVML.create(device_id)
        except AttributeError:
            raise Exception("NVML not installed")
    elif platform == "rapl":
        return pypmt.Rapl.create()
    elif platform == "rocm":
        try:
            return pypmt.ROCM.create(device_id)
        except AttributeError:
            raise Exception("ROCM not installed")
    elif platform == "xilinx":
        return pypmt.Xilinx.create()
    else:
        raise Exception(f"Invalid platform: {platform}")


def measure(platform, device_id=0):
    def decorator(func):
        def wrapper(*args, **kwargs):
            pmt = get_pmt(platform, device_id)

            start = pmt.read()
            func_results = func(*args, **kwargs)
            end = pmt.read()

            results = []
            if func_results is not None:
                results.append(func_results)
            pmt_results = {
                "platform": platform,
                "joules": format(pypmt.PMT.joules(start, end), ".3f"),
                "seconds": format(pypmt.PMT.seconds(start, end), ".3f"),
                "watt": format(pypmt.PMT.watts(start, end), ".3f"),
            }
            results.append(pmt_results)

            return results

        return wrapper

    return decorator


def dump(platform, **kwargs):

    filename = None
    filename_arg = 'filename'
    if filename_arg in kwargs:
        filename = kwargs[filename_arg]

    device_id = 0
    device_id_arg = 'device_id'
    if device_id_arg in kwargs:
        device_id = kwargs[device_id_arg]

    def decorator(func):
        def wrapper(*args, **kwargs):
            pmt = get_pmt(platform, device_id)
            pmt.startDump(filename)
            result = func(*args, **kwargs)
            pmt.stopDump()
            return result

        return wrapper

    return decorator
