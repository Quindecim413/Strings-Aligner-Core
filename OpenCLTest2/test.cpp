#define CL_USE_DEPRECETED_OPENCL_2_0_APIS
#include <CL/cl.hpp>
#include <fstream>

/*int main() {
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);
	auto platform = platforms.front();
	auto el = platform.getInfo<CL_PLATFORM_NAME>();
 	

	std::ifstream helloWorldFile("helloWorld.cl");
	std::string src(std::istreambuf_iterator<char>(helloWorldFile), (std::istreambuf_iterator<char>()));

	cl::Program::Sources sources(1, std::make_pair(src.c_str(), src.length() + 1));

	std::vector<cl::Device> devices;
	platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

	cl::Context context(devices);
	

}*/


