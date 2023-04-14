import pypmt
from pypmt import joules, seconds, watts


def get_pmt(platform, device_id=0):
    if platform == "amdgpu":
        return pypmt.AMDGPU.create(device_id)
    elif platform == "arduino":
        try:
            return pypmt.Arduino.create(device_id)
        except AttributeError:
            raise Exception("Arduino not installed")
    elif platform == "dummy":
        return pypmt.Dummy.create()
    elif platform == "jetson":
        return pypmt.Jetson.create()
    elif platform == "likwid":
        try:
            return pypmt.Likwid.create(device_id)
        except AttributeError:
            raise Exception("Likwid not installed")
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
                "joules": format(pypmt.joules(start, end), ".3f"),
                "seconds": format(pypmt.seconds(start, end), ".3f"),
                "watt": format(pypmt.watts(start, end), ".3f"),
            }
            results.append(pmt_results)

            return results

        return wrapper

    return decorator


def dump(platform, filename, device_id=0):
    def decorator(func):
        def wrapper(*args, **kwargs):
            pmt = get_pmt(platform, device_id)

            if not filename:
                raise Exception("Please provide a filename to dump the results")
            pmt.startDumpThread(filename)
            func(*args, **kwargs)
            pmt.stopDumpThread()

            return

        return wrapper

    return decorator
