#include <iostream>

#include "color.h"
#include "ray.h"
#include "vec3.h"

color ray_color(const ray& r) { return color(0, 0, 0); }

int main() {
  // render target
  double aspect_ratio = 16.0 / 9.0;
  int image_width = 400;
  int image_height = static_cast<int>(image_width / aspect_ratio);
  if (image_height < 1) {
    image_height = 1;
  }

  // camera
  double focal_length = 1.0;
  double viewport_height = 2.0;
  double viewport_width = viewport_height * (static_cast<double>(image_width) /
                                             static_cast<double>(image_height));
  point3 origin(0, 0, 0);

  vec3 viewport_u(viewport_width, 0, 0);
  vec3 viewport_v(0, -viewport_height, 0);

  vec3 pixel_du = viewport_u / static_cast<double>(image_width);
  vec3 pixel_dv = viewport_v / static_cast<double>(image_height);

  // viewport upper left corner
  vec3 viewport_upper_left =
      origin - vec3(0, 0, focal_length) - viewport_u * 0.5 - viewport_v * 0.5;
  // position of the pixel (0, 0)
  vec3 pixel00 = viewport_upper_left + pixel_du * 0.5 + pixel_dv * 0.5;

  // render
  std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

  for (int j = 0; j < image_height; ++j) {
    std::clog << "\rScanlines remaining:\t" << image_height - j << ' '
              << std::flush;
    for (int i = 0; i < image_width; ++i) {
      vec3 pixel_center = pixel00 + pixel_du * i + pixel_dv * j;
      ray r(origin, pixel_center - origin);
      color pixel_color = ray_color(r);
      write_color(std::cout, pixel_color);
    }
  }

  std::clog << "\rDone.                          \n";
}
