/*
    Read the RAPL values from the sysfs powercap interface

    Original code by Vince Weaver (vincent.weaver@maine.edu)
    http://web.eece.maine.edu/~vweaver/projects/rapl/rapl-read.c

    Updated with code taken from:
    https://github.com/deater/uarch-configure/blob/master/rapl-read/rapl-read.c
*/

#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "rapl-read.h"

#define CPU_VENDOR_INTEL 1
#define CPU_VENDOR_AMD 2

#define CPU_SANDYBRIDGE 42
#define CPU_SANDYBRIDGE_EP 45
#define CPU_IVYBRIDGE 58
#define CPU_IVYBRIDGE_EP 62
#define CPU_HASWELL 60
#define CPU_HASWELL_ULT 69
#define CPU_HASWELL_GT3E 70
#define CPU_HASWELL_EP 63
#define CPU_BROADWELL 61
#define CPU_BROADWELL_GT3E 71
#define CPU_BROADWELL_EP 79
#define CPU_BROADWELL_DE 86
#define CPU_SKYLAKE 78
#define CPU_SKYLAKE_HS 94
#define CPU_SKYLAKE_X 85
#define CPU_KNIGHTS_LANDING 87
#define CPU_KNIGHTS_MILL 133
#define CPU_KABYLAKE_MOBILE 142
#define CPU_KABYLAKE 158
#define CPU_ATOM_SILVERMONT 55
#define CPU_ATOM_AIRMONT 76
#define CPU_ATOM_MERRIFIELD 74
#define CPU_ATOM_MOOREFIELD 90
#define CPU_ATOM_GOLDMONT 92
#define CPU_ATOM_GEMINI_LAKE 122
#define CPU_ATOM_DENVERTON 95

#define CPU_AMD_FAM17H 0xc000

using namespace raplread;

Rapl::Rapl() {
  // Detect cpu and packages
  detect_cpu();
  detect_packages();

// Check whether rapl works
#if defined(DEBUG)
  measure();
  if (!rapl_supported) {
    fprintf(stderr,
            "RAPL not supported, power measurement will be unavailable.\n");
  }
#endif
}

void Rapl::detect_cpu() {
  FILE *fff;

  int vendor = -1, family, model = -1;
  char buffer[BUFSIZ], *result;
  char vendor_string[BUFSIZ];

  fff = fopen("/proc/cpuinfo", "r");
  if (fff == NULL) {
    rapl_supported = false;
  }

  while (1) {
    result = fgets(buffer, BUFSIZ, fff);
    if (result == NULL) break;

    if (!strncmp(result, "vendor_id", 8)) {
      sscanf(result, "%*s%*s%s", vendor_string);

      if (!strncmp(vendor_string, "GenuineIntel", 12)) {
        vendor = CPU_VENDOR_INTEL;
      }
      if (!strncmp(vendor_string, "AuthenticAMD", 12)) {
        vendor = CPU_VENDOR_AMD;
      }
    }

    if (!strncmp(result, "cpu family", 10)) {
      sscanf(result, "%*s%*s%*s%d", &family);
    }

    if (!strncmp(result, "model", 5)) {
      sscanf(result, "%*s%*s%d", &model);
    }
  }

  if (vendor == CPU_VENDOR_INTEL) {
    if (family != 6) {
      printf("Wrong CPU family %d\n", family);
      rapl_supported = false;
    }

#if defined(DEBUG)
    printf("CPU model: ");

    switch (model) {
      case CPU_SANDYBRIDGE:
        printf("Sandybridge");
        break;
      case CPU_SANDYBRIDGE_EP:
        printf("Sandybridge-EP");
        break;
      case CPU_IVYBRIDGE:
        printf("Ivybridge");
        break;
      case CPU_IVYBRIDGE_EP:
        printf("Ivybridge-EP");
        break;
      case CPU_HASWELL:
      case CPU_HASWELL_ULT:
      case CPU_HASWELL_GT3E:
        printf("Haswell");
        break;
      case CPU_HASWELL_EP:
        printf("Haswell-EP");
        break;
      case CPU_BROADWELL:
      case CPU_BROADWELL_GT3E:
        printf("Broadwell");
        break;
      case CPU_BROADWELL_EP:
        printf("Broadwell-EP");
        break;
      case CPU_SKYLAKE:
      case CPU_SKYLAKE_HS:
        printf("Skylake");
        break;
      case CPU_SKYLAKE_X:
        printf("Skylake-X");
        break;
      case CPU_KABYLAKE:
      case CPU_KABYLAKE_MOBILE:
        printf("Kaby Lake");
        break;
      case CPU_KNIGHTS_LANDING:
        printf("Knight's Landing");
        break;
      case CPU_KNIGHTS_MILL:
        printf("Knight's Mill");
        break;
      case CPU_ATOM_GOLDMONT:
      case CPU_ATOM_GEMINI_LAKE:
      case CPU_ATOM_DENVERTON:
        printf("Atom");
        break;
      default:
        printf("Unsupported model %d\n", model);
        model = -1;
        break;
    }
    printf("\n");
#endif
  }

  if (vendor == CPU_VENDOR_AMD) {
    if (!(family == 23 ||   // Zen, Zen+, Zen2
          family == 25)) {  // Zen 3
      printf("Wrong CPU family %d\n", family);
      exit(1);
    }
    model = CPU_AMD_FAM17H;
  }

  fclose(fff);
  cpu_model = model;
}  // end Rapl::detect_cpu

void Rapl::detect_packages() {
  char filename[BUFSIZ];
  FILE *fff;
  int package;
  int i;

  for (i = 0; i < MAX_PACKAGES; i++) package_map[i] = -1;

#if defined(DEBUG)
  printf("\t");
#endif
  for (i = 0; i < MAX_CPUS; i++) {
    sprintf(filename,
            "/sys/devices/system/cpu/cpu%d/topology/physical_package_id", i);
    fff = fopen(filename, "r");
    if (fff == NULL) break;
    fscanf(fff, "%d", &package);
#if defined(DEBUG)
    printf("%d (%d)", i, package);
    if (i % 8 == 7)
      printf("\n\t");
    else
      printf(", ");
#endif
    fclose(fff);

    if (package_map[package] == -1) {
      total_packages++;
      package_map[package] = i;
    }
  }

#if defined(DEBUG)
  printf("\n");
#endif

  total_cores = i;

#if defined(DEBUG)
  printf("\tDetected %d cores in %d packages\n\n", total_cores, total_packages);
#endif
}  // end Rapl::detect_packages

double Rapl::measure() {
  char filenames[MAX_PACKAGES][NUM_RAPL_DOMAINS][256];
  char basename[MAX_PACKAGES][256];
  char tempfile[256];
  long long measurements[MAX_PACKAGES][NUM_RAPL_DOMAINS];
  int valid[MAX_PACKAGES][NUM_RAPL_DOMAINS];
  int i, j;
  FILE *fff;

  if (!rapl_supported) {
    return 0;
  }

  double consumed_energy_dram = 0;
  double consumed_energy_package = 0;

  /* /sys/class/powercap/intel-rapl/intel-rapl:0/ */
  /* name has name */
  /* energy_uj has energy */
  /* subdirectories intel-rapl:0:0 intel-rapl:0:1 intel-rapl:0:2 */
  for (j = 0; j < total_packages; j++) {
    i = 0;
    sprintf(basename[j], "/sys/class/powercap/intel-rapl/intel-rapl:%d", j);
    sprintf(tempfile, "%s/name", basename[j]);
    fff = fopen(tempfile, "r");
    if (fff == NULL) {
      rapl_supported = false;
      return 0;
    }
    valid[j][i] = 1;
    fclose(fff);
    sprintf(filenames[j][i], "%s/energy_uj", basename[j]);

    /* Handle subdomains */
    for (i = 1; i < NUM_RAPL_DOMAINS; i++) {
      sprintf(tempfile, "%s/intel-rapl:%d:%d/name", basename[j], j, i - 1);
      fff = fopen(tempfile, "r");
      if (fff == NULL) {
        valid[j][i] = 0;
        continue;
      }
      valid[j][i] = 1;
      fclose(fff);
      sprintf(filenames[j][i], "%s/intel-rapl:%d:%d/energy_uj", basename[j], j,
              i - 1);
    }
  }

  /* Gather measurements */
  for (j = 0; j < total_packages; j++) {
    for (i = 0; i < NUM_RAPL_DOMAINS; i++) {
      if (valid[j][i]) {
        fff = fopen(filenames[j][i], "r");
        if (fff == NULL) {
          rapl_supported = false;
#if defined(DEBUG)
          fprintf(stderr, "\tError opening %s!\n", filenames[j][i]);
#endif
        } else {
          fscanf(fff, "%lld", &measurements[j][i]);
          fclose(fff);
        }
      }
    }
  }

  // Compute package and dram power consumption from individual measurements
  for (j = 0; j < total_packages; j++) {
    for (i = 0; i < NUM_RAPL_DOMAINS; i++) {
      if (valid[j][i]) {
        long long value = measurements[j][i] * 1e-6;
        // RAPL_DOMAIN 0: package
        if (i == 0) {
          consumed_energy_package += value;

          // RAPL_DOMAIN 1: dram
        } else if (i == 1) {
          consumed_energy_dram += value;
        }
      }
    }
  }

  rapl_supported = true;
  return consumed_energy_package + consumed_energy_dram;
}  // end Rapl::measure
