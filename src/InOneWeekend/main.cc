#include <iostream>

#include "color.h"
#include "ray.h"
#include "vec3.h"

double hit_sphere(const point3& center, double radius, const ray& r) {
  vec3 oc = center - r.origin();
  double a = r.direction().length_squared();
  double h = dot(oc, r.direction());
  double c = oc.length_squared() - radius * radius;
  double discriminant = h * h - a * c;

  if (discriminant < 0) {
    return -1.0;
  } else {
    return (h - std::sqrt(discriminant)) / a;
  }
}

color ray_color(const ray& r) {
  vec3 O(0, 0, -1);
  double t = hit_sphere(O, 0.5, r);
  if (t > 0.0) {
    vec3 N = unit_vector(r.at(t) - O);
    return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
  }

  vec3 unit_direction = unit_vector(r.direction());
  double a = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

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
