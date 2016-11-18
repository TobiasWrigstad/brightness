#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <libgen.h>

#define EXIT_CODE int

#define Backlight_file "/sys/class/backlight/intel_backlight/brightness"
#define Min 1
#define Max 937

void help()
{
  puts("Usage: tb [--set N|--inc N|--dec N|--get]");
}

static void set_current_value(int value)
{
  FILE *f = fopen(Backlight_file, "w");

  if (f)
    {
      fprintf(f, "%d\n", value);
      fclose(f);
    }
  else
    {
      fprintf(stderr, "Error: could not get handle to %s.\nProbably permissions problem!\n", Backlight_file);
      return; 
    }  
}

static int get_current_value()
{
  int current_brightness = -1;

  FILE *f = fopen(Backlight_file, "r");
  int read = fscanf(f, "%d", &current_brightness);
  fclose(f);

  assert(read == 1);
  assert(current_brightness >= 0);
  assert(current_brightness <= Max); /// Apparently max for device

  return current_brightness;
}

int adjust_within_bounds(int value)
{
  if (value < Min) return Min;
  if (value > Max) return Max;
  return value;
}

EXIT_CODE get()
{
  printf("%d\n", get_current_value());
  return 0;
}

EXIT_CODE set(int brightness)
{
  if (brightness < Min || brightness > Max)
    {
      fprintf(stderr, "Error: illegal value. Must be within [%d,%d]\n", Min, Max);
      
      return -1;
    }

  set_current_value(brightness);

  return 0;
}

EXIT_CODE inc(int brightness)
{
  int target = get_current_value() + brightness;
  
  return set(adjust_within_bounds(target));
}

EXIT_CODE dec(int brightness)
{
  int target = get_current_value() - brightness;
  
  return set(adjust_within_bounds(target));
}

int main(int argc, char *argv[])
{
  switch (argc)
    {
    case 1:
      {
        /// Special -- if screen is BLACK, running the program
        /// without arguments should turn on minmal light
        if (strcmp("bt", basename(argv[0])) == 0 && get_current_value() == 0)
          {
            set(Min);
            return 0;
          }
        /// Case: program name controls behaviour
        puts("Not yet supported.");
        return -1;
      }
    case 2:
      {
        /// Possibilities: --get
        /// Possibilities: --BLACK
        /// Possibilities: --help
        if (strcmp("--get", argv[1]) == 0)
          {
            return get();
          }
        else
        if (strcmp("--BLACK", argv[1]) == 0)
          {
            /// Special -- allows overriding Min to really blacken screen
            set_current_value(0);
            return 0;
          }
        else
        if (strcmp("--help", argv[1]) == 0)
          {
            help();
            return 0;
          }
      }
    case 3:
      {
        /// Possibilities: -set N
        /// Possibilities: -inc N
        /// Possibilities: -det N
        int value = atoi(argv[2]);
        if (errno)
          {
            printf("Illegal parameter to %s: %s\n", argv[1], argv[2]);
            return -1;
          }
        
        if (strcmp("--set", argv[1]) == 0)
          {
            return set(value);
          }
        else
        if (strcmp("--inc", argv[1]) == 0)
          {
            inc(value);
            return 0;
          }
        else
        if (strcmp("--dec", argv[1]) == 0)
          {
            dec(value);
            return 0;
          }
        /// NOTE: continues to default action!
      }
    default:
      {
        puts("Illegal parmeters.");
      }
    }
  return 0;
}
