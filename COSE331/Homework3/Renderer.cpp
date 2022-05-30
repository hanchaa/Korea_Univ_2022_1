# include <GLFW/glfw3.h>
# include <glm/vec3.hpp>
# include <glm/vec4.hpp>
# include <glm/mat4x4.hpp>

# include <stdlib.h>
# include <stdio.h>
# include <vector>
#define STB_IMAGE_IMPLEMENTATION
# include "stb_image.h"

using namespace std;
using namespace glm;
struct HitData
{
	bool is_hit;
	vec3 position;
	vec3 normal;
	float t; 
	bool is_front;
	int object_index; //100 for floor, -1 for none

	//for floor texture
	float u, v;
};
struct Material
{
	vec3 color; //material diffuse color (same for ambient, specular)
	float diffuse, ambient, specular;
	float n;
	float reflection, refraction; 
};



vector<float> sphere_radii;
vector<vec3> sphere_positions;
vector<Material> sphere_materials;
Material floor_material;

// You can modify the window resolution here.
int WINDOW_WIDTH = 640;
int WINDOW_HEIGHT = 480;
const int RAY_MAX_DEPTH = 5;
int TEXTURE_IMAGE_WIDTH;
int TEXTURE_IMAGE_HEIGHT;

vec3 LIGHT_POS = vec3(12.0f, 8.0f, 30.0f);
vector<vector<vec3>> texture_array;
vector<vector<vec3>> normal_array;
vector<vector<vec3>> height_array;

vec3 get_background_color(vec3 r)
{
	float t = 0.5f * (r.y + 1.0f);

	//return (1.0f - t) * vec3(0.7, 0.4, 0.3) + t * vec3(0.1, 0.1, 0.5); //twilight mode ^^7
	return (1.0f - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.3, 0.5, 0.9);

	//return vec3(1.0f);
}
vec3 texture_interpolate(vector<vector<vec3>>& texture, float u, float v)
{

	float scale = TEXTURE_IMAGE_WIDTH < TEXTURE_IMAGE_HEIGHT ? TEXTURE_IMAGE_WIDTH : TEXTURE_IMAGE_HEIGHT;
	float pixu = (u * scale);
	float pixv = (v * scale);
	int ui = (int)pixu;
	int ui_1 = (int)pixu + 1;
	int vi = (int)pixv;
	int vi_1 = (int)pixv + 1;
	ui %= TEXTURE_IMAGE_WIDTH;
	ui_1 %= TEXTURE_IMAGE_WIDTH;
	vi %= TEXTURE_IMAGE_HEIGHT;
	vi_1 %= TEXTURE_IMAGE_HEIGHT;
	float ud = pixu - ui;
	float vd = pixv - vi;
	vec3 tex = texture[ui][vi] * (1 - ud) * (1 - vd) \
		+ texture[ui_1][vi] * ud * (1 - vd) \
		+ texture[ui][vi_1] * (1 - ud) * vd \
		+ texture[ui_1][vi_1] * ud * vd;
	return tex;
}
HitData plane_hit(vec3 corner, float t_min, float t_max, vec3 side_1, vec3 side_2, vec3 eye, vec3 ray)
{

	HitData hit;
	hit.is_hit = false;


	float denominator = (dot(cross(normalize(ray), side_1), side_2));
	float l1 = sqrt(dot(side_1, side_1));
	float l2 = sqrt(dot(side_2, side_2));
	float t, u, v;

	if (abs(denominator) <= 1e-6)
	{
		return hit;
	}

	t = -(dot(cross(eye - corner, side_1), side_2)) / denominator;
	u = (dot(cross(normalize(ray), eye - corner), side_2)) / denominator;
	v = (dot(cross(normalize(ray), side_1), eye - corner)) / denominator;

	if (t <= t_max && t >= t_min)
	{
		if (u >= 0 && u <= 1 && v >= 0 && v <= 1)
		{
			hit.t = t;
			hit.position = eye + ray * hit.t;

			hit.is_hit = true;
			hit.is_front = true;

			hit.u = u;
			hit.v = v;

			vec3 texture_normal = normalize(texture_interpolate(normal_array, u, v));
			hit.normal = normalize(side_1) * texture_normal.x + normalize(side_2) * texture_normal.y + normalize(cross(side_1, side_2)) * texture_normal.z;
			hit.object_index = 100;
			return hit;
		}
	}
	return hit;
}
HitData sphere_hit(vec3 center, float t_min, float t_max, float radius, vec3 eye, vec3 ray)
{
	vec3 oc = eye - center;
	float a = dot(ray, ray);
	float b = 2.0f * dot(oc, ray);
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4.0f * a * c;

	HitData hit;
	hit.is_hit = false;
	if (discriminant > 0)
	{
		float temp = (-b - sqrt(discriminant)) / (2.0f * a);
		if (temp <= t_max && temp >= t_min)
		{
			hit.t = temp;
			hit.position = eye + ray * hit.t;
			hit.normal = (hit.position - center) / radius;
			hit.is_hit = true;
			hit.is_front = true;
			return hit;
		}
		temp = (-b + sqrt(discriminant)) / (2.0f * a);
		if (temp <= t_max && temp >= t_min)
		{
			hit.t = temp;
			hit.position = eye + ray * hit.t;
			hit.normal = (hit.position - center) / radius;
			hit.is_hit = true;
			hit.is_front = false;
			return hit;
		}
	}
	return hit;
}
HitData get_closest_hit(vec3 eye, vec3 ray) {

	HitData closest_hit;
	closest_hit.object_index = -1;

	float t_min = 9999.f;
	for (int i = 0; i < sphere_positions.size(); i++)
	{
		HitData hit = sphere_hit(sphere_positions[i], 0.01f, t_min, sphere_radii[i], eye, ray);
		if (hit.is_hit == true)
		{
			if (hit.t < t_min)
			{
				t_min = hit.t;
				closest_hit = hit;
				closest_hit.object_index = i;


			}
		}
	}

	HitData hit = plane_hit(vec3(-15, -3, -15), 0.01f, t_min, vec3(0, 0, 30), vec3(30, 0, 0), eye, ray);
	if (hit.is_hit == true)
	{
		if (hit.t < t_min)
		{
			t_min = hit.t;
			closest_hit = hit;

		}

	}

	return closest_hit;


}
vec3 get_phong_color(vec3 input_ray, HitData hit, Material m) {
	vec3 light_ray = normalize(LIGHT_POS - hit.position);
	vec3 ray_normal = dot(input_ray, hit.normal) * hit.normal;
	vec3 R = input_ray - 2.0f * ray_normal;

	float diffuse = m.diffuse * std::max(0.0f, dot(light_ray, hit.normal));
	float ambient = m.ambient;
	float specular = m.specular * pow(std::max(dot(light_ray, R), 0.f), 8);

	return m.color * (ambient + diffuse + specular);

}



vec3 get_reflection_ray(vec3 input_ray, HitData hit) {

	//TODO: Problem1
	return input_ray;
}
vec3 get_refraction_ray(vec3 input_ray, HitData hit, Material m) {

	//TODO: Problem2
	return input_ray;

}

bool is_lighted(vec3 eye) {

	//TODO: Problem3

	return true;
}
void construct_normal_map(vector<vector<vec3>>& img_height, vector<vector<vec3>>& img_normal)
{
	//TODO: Problem4
	for (int x = 0; x < TEXTURE_IMAGE_WIDTH-1; x++)
	{
		for (int y = 0; y < TEXTURE_IMAGE_HEIGHT-1; y++)
		{

			img_normal[x][y] = vec3(0, 0, 1);
		}
	}


}
vec3 get_shadow_ray(vec3 eye) {

	bool light_hit = is_lighted(eye);

	return vec3(light_hit ? 0.1f : 0);
}


vec3 get_color(int depth, vec3 eye, vec3 ray)
{
	if (depth > RAY_MAX_DEPTH)
	{
		return get_background_color(ray);
	}


	HitData closest_hit = get_closest_hit(eye, ray);

	//if the ray intersects with objects.
	if (closest_hit.object_index >= 0)
	{
		vec3 color(0.0f);
		Material m;

		//sphere material setting
		if (closest_hit.object_index != 100) m = sphere_materials[closest_hit.object_index];

		//floor mateiral setting
		else {
			m = floor_material;
			//set color from texture
			m.color = texture_interpolate(texture_array, closest_hit.u, closest_hit.v);
		}
	
		if (dot(ray, closest_hit.normal) > 0.0f)
			closest_hit.normal = -closest_hit.normal;

		color = get_phong_color(ray, closest_hit, m);
		eye = closest_hit.position;


		vec3 reflected_ray = get_reflection_ray(ray, closest_hit);
		vec3 refracted_ray = get_refraction_ray(ray, closest_hit, m);
		vec3 shadow_ray = get_shadow_ray(eye);

		//get recursively collected rays
		return (m.reflection * get_color(depth + 1, eye, reflected_ray) + m.refraction * get_color(depth + 1, eye, refracted_ray) + shadow_ray) * color;
	}


	return get_background_color(ray);
}


void init() {
	sphere_radii = { 2,4,3,0.8,0.8,0.8,2,1,1,1,1,0.8,0.8};
	sphere_positions = {
	{8.0f, -3.0f, -1.0f},
	{-0.0f, -3.0f, 0.0f},
	{-9.0f, -3.0f, -8.0f},
	{-2.0f, -3.0f, 7.0f},
	{-4.0f, -3.0f, 8.0f},
	{2.0f, -3.0f, 9.0f},
	{-6.0f, -3.0f, 5.0f},
	{9.0f, -3.0f, 4.0f},
	{-3.0f, -3.0f, -8.0f},
	{2.0f, -3.0f, -7.0f},
	{6.0f, -3.0f, -8.0f},
	{8.0f, -3.0f, -8.0f},
	{6.0f, -3.0f, 5.0f}
	};
	sphere_materials = {
	{{1.0f, 1.0f, 1.0f}, 0.01f, 1.0f, 0.5f, 1.333f, 0.98f, 0.01f}, //mirror
	{{1.0f, 1.0f, 1.0f}, 0.01f, 0.9f, 0.4f, 7.0f, 0.0f, 1.0f}, //lens
	{{0.882f,0.376f,0.212f}, 0.3f, 0.88f, 0.7f, 1.333f, 0.1f, 0.0f},
	{{0.922f,0.22f,0.322f}, 0.5f, 0.5f, 0.7f, 1.333f, 0.1f, 0.9f},
	{{0.502f,0.929f,0.455f}, 0.5f, 0.88f, 0.7f, 1.333f, 0.01f, 0.01f}, 
	{{0.247,0.533,0.961}, 0.5f, 0.88f, 0.7f, 1.333f, 0.01f, 0.01f}, 
	{{0.949f,0.894f,0.122f}, 0.3f, 0.9f, 0.4f, 2.0f, 0.5f, 0.5f},
	{{0.384f,0.361f,0.953f}, 0.5f, 0.5f, 0.7f, 1.333f, 0.1f, 0.5f},
	{{0.267,0.086,0.961}, 1.0f, 0.5f, 0.7f, 1.333f, 0.8f, 0.1f},
	{{0.086,1.,0.498}, 0.3f, 0.88f, 0.7f, 1.333f, 0.1f, 0.0f},
	{{1.,0.267,0.176}, 0.5f, 0.5f, 0.7f, 1.333f, 0.1f, 0.5f},
	{{0.11,0.902,0.875}, 1.0f, 0.88f, 0.7f, 1.333f, 0.1f, 0.1f},
	{{0.471,1.,0.122}, 1.0f, 0.5f, 0.7f, 1.333f, 0.8f, 0.1f}
	};
	floor_material = {
	vec3(1,1,1),
	0.9f,0.9f,0.1f,
	1.333f,
	0.02f,0.f
	};

	for (int i = 0; i < sphere_positions.size(); i++)
	{
		sphere_positions[i].y = -3 + sphere_radii[i];
	}
}

void draw()
{

	static vec3* pixels = new vec3[WINDOW_WIDTH * WINDOW_HEIGHT];
#pragma omp parallel for
	for (int x = 0; x < WINDOW_WIDTH; x++)
	{
		for (int y = 0; y < WINDOW_HEIGHT; y++)
		{
			int index = x + y * WINDOW_WIDTH;

			float ray_x = (float)x / WINDOW_HEIGHT - 0.5f * WINDOW_WIDTH / WINDOW_HEIGHT;
			float ray_y = (float)y / WINDOW_HEIGHT - 0.5f;;

			float ray_z = -0.5;


			vec3 ray = normalize(vec3(ray_x, ray_y, ray_z));;
			vec3 eye = vec3(0.0f, 0.0f, 15.0f);

			vec3 color = get_color(0, eye, ray);
			// gamma-correction r = 1/2
			pixels[index] = sqrt((color));
		}
	}

	glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, pixels);
	glFlush();

}

void load_image(const char* filename, vector<vector<vec3>>& img_rgb)
{

	int channels;

	unsigned char* img = stbi_load(filename, &TEXTURE_IMAGE_WIDTH, &TEXTURE_IMAGE_HEIGHT, &channels, 0);
	//vec3* texture_pixels = new vec3[width * height];
	img_rgb = vector<vector<vec3>>(TEXTURE_IMAGE_WIDTH, vector<vec3>(TEXTURE_IMAGE_HEIGHT, vec3(0, 0, 0)));
	for (int y = 0; y < TEXTURE_IMAGE_HEIGHT; ++y) {

		for (int x = 0; x < TEXTURE_IMAGE_WIDTH; ++x) {
			int index = x + y * TEXTURE_IMAGE_WIDTH;


			img_rgb[x][y] = vec3(img[channels * index] / 255., img[channels * index + 1] / 255., img[channels * index + 2] / 255.);

		}

	}

	if (img == NULL) {
		printf("Error in loading the image\n");
		exit(1);

	}
	printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", TEXTURE_IMAGE_WIDTH, TEXTURE_IMAGE_HEIGHT, channels);

	stbi_image_free(img);
}

int main()
{
	GLFWwindow* window;

	if (!glfwInit())
	{
		printf("Error: Initialize GLFW failure\n");;
		exit(EXIT_FAILURE);
	}

	//Windows VS
	load_image("./res/img.png", texture_array);
	load_image("./res/heightmap.png", height_array);

	//macOS xcode
	//load_image("../../../res/img.png", texture_array);
	//load_image("../../../res/heightmap.png", height_array);

	normal_array = vector<vector<vec3>>(TEXTURE_IMAGE_WIDTH, vector<vec3>(TEXTURE_IMAGE_HEIGHT, vec3(0, 0, 1)));
	construct_normal_map(height_array, normal_array);


	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "HW3", NULL, NULL);
	glfwGetFramebufferSize(window, &WINDOW_WIDTH, &WINDOW_HEIGHT);

	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	//srand(3211);

	init();
	draw();
	glfwSwapBuffers(window);
	draw();


	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}