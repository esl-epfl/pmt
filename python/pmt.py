import pypmt
from pypmt import joules, seconds, watts


def get_pmt(platform, device_id=0):
    if platform in ['powersensor2', 'powersensor3']:
        try:
            return pypmt.PowerSensor_.create(device_id, 2)
        except AttributeError:
            raise Exception("PowerSensor2 not installed")
    elif platform == 'powersensor3':
        try:
            return pypmt.PowerSensor_.create(device_id, 3)
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
            result = func(*args, **kwargs)
            pmt.stopDumpThread()

            return result

        return wrapper

    return decorator
