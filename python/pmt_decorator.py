import sys
import time

import pypmt as ps

def pmt(platform, device_id = 0):
    def decorator(func):
        def wrapper(*args, **kwargs):
            if platform == "amdgpu":
                pmt = ps.AMDGPU.create(device_id)
            elif platform == "arduino":
                try:
                    pmt = ps.Arduino.create(device_id)
                except AttributeError:
                    print("Arduino not installed")
                    sys.exit(1)
            elif platform == "dummy":
                pmt = ps.Dummy.create()
            elif platform == "jetson":
                pmt = ps.Jetson.create()
            elif platform == "likwid":
                try:
                    pmt = ps.Likwid.create(device_id)
                except AttributeError:
                    print("Likwid not installed")
                    sys.exit(1)
            elif platform == "nvml":
                try:
                    pmt = ps.NVMLpmt.create(device_id)
                except AttributeError:
                    print("NVMLpmt not installed")
                    sys.exit(1)
            elif platform == "rapl":
                pmt = ps.Raplpmt.create()
            elif platform == "rocm":
                try: 
                    pmt = ps.ROCMpmt.create(device_id)
                except AttributeError:
                    print("ROCMpmt not installed")
                    sys.exit(1)
            elif platform == "xilinx":
                pmt = ps.Xilinxpmt.create()
            else:
                print("Invalid Option")
                sys.exit(1)

            start = pmt.read()
            prev_results = func(*args, **kwargs)
            end = pmt.read()

            results = []
            if prev_results is not None:
                results.append(prev_results[0])
            curr_results = {"platform" : platform,  \
                            "joules" : format(ps.joules(start, end),".3f"),  \
                            "seconds" : format(ps.seconds(start, end),".3f"), \
                            "watt" : format(ps.watts(start, end),".3f")}
            results.append(curr_results)

            return results
        return wrapper
    return decorator

def pmt_dump(platform, filename, device_id = 0):
    def decorator(func):
        def wrapper(*args, **kwargs):
            if platform == "amdgpu":
                pmt = ps.AMDGPU.create(device_id)
            elif platform == "arduino":
                try:
                    pmt = ps.Arduino.create(device_id)
                except AttributeError:
                    print("Arduino not installed")
                    sys.exit(1)
            elif platform == "dummy":
                pmt = ps.Dummy.create()
            elif platform == "jetson":
                pmt = ps.Jetson.create()
            elif platform == "likwid":
                try:
                    pmt = ps.Likwid.create(device_id)
                except AttributeError:
                    print("Likwid not installed")
                    sys.exit(1)
            elif platform == "nvml":
                try:
                    pmt = ps.NVMLpmt.create(device_id)
                except AttributeError:
                    print("NVMLpmt not installed")
                    sys.exit(1)
            elif platform == "rapl":
                pmt = ps.Raplpmt.create()
            elif platform == "rocm":
                try: 
                    pmt = ps.ROCMpmt.create(device_id)
                except AttributeError:
                    print("ROCMpmt not installed")
                    sys.exit(1)
            elif platform == "xilinx":
                pmt = ps.Xilinxpmt.create()
            else:
                print("Invalid Option")
                sys.exit(1)

            if(not filename):
                print("Please provide a filename to dump the results")
                sys.exit(1)
            start = pmt.startDumpThread(filename)
            prev_results = func(*args, **kwargs)
            end = pmt.stopDumpThread()

            return
        return wrapper
    return decorator

