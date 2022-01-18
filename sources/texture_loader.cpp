#include "texture_loader.hpp"

////////////////////////////////////////////////////////////////////////////////

Scop::TextureLoader::TextureLoader(ft::String path_to_png) {
	this->file_content = nullptr;
	this->img = nullptr;
	readFile(path_to_png);
	parseFile();
}

void Scop::TextureLoader::readFile(ft::String path) {
	ft::String::size_type n = path.rfind(".bmp");

	if (n == ft::String::npos
		|| path.length() <= 4
		|| n + 4 != path.length()
	) {
		throw Scop::TextureLoaderException("Wrong format of file: " + path);
	}


	std::ifstream file;
	file.open(path.c_str(), std::ios::in | std::ios::binary);

	if (!file.is_open()) {
		throw Scop::TextureLoaderException("Fail to open file: " + path);
	}

	file.seekg(0, std::ios::end);
	this->content_size = file.tellg();
	this->file_content = new unsigned char [this->content_size];
	file.seekg(0, std::ios::beg);
	file.read(reinterpret_cast<char*>(this->file_content), this->content_size);

	file.close();
}

void Scop::TextureLoader::parseFile() {
	if (file_content[0] != 0x42 || file_content[1] != 0x4D) {
		throw TextureLoaderException("File corrupted");
	}

	size_t b1 = file_content[2];
	size_t b2 = file_content[3];
	size_t b3 = file_content[4];
	size_t b4 = file_content[5];

	size_t bmp_size = (b4 << 24) + (b3 << 16) + (b2 << 8) + b1;
	if (bmp_size != this->content_size) {
		throw TextureLoaderException("File corrupted");
	}

	b1 = file_content[10];
	b2 = file_content[11];
	b3 = file_content[12];
	b4 = file_content[13];
	size_t data_offset = (b4 << 24) + (b3 << 16) + (b2 << 8) + b1;

	b1 = file_content[14];
	b2 = file_content[15];
	b3 = file_content[16];
	b4 = file_content[17];
	size_t dib_size = (b4 << 24) + (b3 << 16) + (b2 << 8) + b1;

	b1 = file_content[18];
	b2 = file_content[19];
	b3 = file_content[20];
	b4 = file_content[21];
	this->img_width = (b4 << 24) + (b3 << 16) + (b2 << 8) + b1;

	b1 = file_content[22];
	b2 = file_content[23];
	b3 = file_content[24];
	b4 = file_content[25];
	this->img_height = (b4 << 24) + (b3 << 16) + (b2 << 8) + b1;

	if (this->img_height > this->img_width) {
		b1 = file_content[22];
		b2 = file_content[23];
		b3 = file_content[24];
		b4 = file_content[25];
		this->img_height = ~(b4 << 24) + ~(b3 << 16) + ~(b2 << 8) + ~b1;
	}

	if (file_content[26] != 1 || file_content[27] != 0) {
		throw TextureLoaderException("File corrupted");
	}

	b1 = file_content[28];
	b2 = file_content[29];
	size_t bits_per_pixel = (b2 << 8) + b1;

	if (bits_per_pixel != 24) {
		throw TextureLoaderException("Only 24 bits image");
	}

	if (file_content[30] != 0 || file_content[31] != 0
		|| file_content[32] != 0 || file_content[33] != 0
	) {
		throw TextureLoaderException("Only RGB");
	}

	b1 = file_content[34];
	b2 = file_content[35];
	b3 = file_content[36];
	b4 = file_content[37];
	this->img_size = (b4 << 24) + (b3 << 16) + (b2 << 8) + b1;

	if (img_size == 0) {
		this->img_size = this->img_width * this->img_height * 3;
	}

	this->img = new unsigned char [this->img_size];
	for (size_t i = 0; i < this->img_size; i++) {
		this->img[i] = file_content[i + data_offset];
	}
}

////////////////////////////////////////////////////////////////////////////////

Scop::TextureLoader::~TextureLoader() {
	delete[] this->file_content;
	delete[] this->img;
}

Scop::TextureLoader::TextureLoader(TextureLoader &rhs) {
	delete[] this->file_content;
	delete[] this->img;
	this->path = rhs.path;

	this->file_content = new unsigned char [rhs.content_size];
	for (size_t i = 0; i < rhs.content_size; i++) {
		this->file_content[i] = rhs.file_content[i];
	}
	this->content_size = rhs.content_size;

	this->img = new unsigned char [rhs.img_size];
	for (size_t i = 0; i < rhs.img_size; i++) {
		this->img[i] = rhs.img[i];
	}
	this->img_size = rhs.img_size;
	this->img_width = rhs.img_width;
	this->img_height = rhs.img_height;
}

Scop::TextureLoader Scop::TextureLoader::operator=(TextureLoader &rhs) {
	if (this == &rhs) {
		return (*this);
	}
	delete[] this->file_content;
	delete[] this->img;
	this->path = rhs.path;

	this->file_content = new unsigned char [rhs.content_size];
	for (size_t i = 0; i < rhs.content_size; i++) {
		this->file_content[i] = rhs.file_content[i];
	}
	this->content_size = rhs.content_size;

	this->img = new unsigned char [rhs.img_size];
	for (size_t i = 0; i < rhs.img_size; i++) {
		this->img[i] = rhs.img[i];
	}
	this->img_size = rhs.img_size;
	this->img_width = rhs.img_width;
	this->img_height = rhs.img_height;
	return (*this);
}

unsigned char* Scop::TextureLoader::getPixelArray() {
	return (this->img);
}

size_t Scop::TextureLoader::getWidth() {
	return (this->img_width);
}

size_t Scop::TextureLoader::getHeight() {
	return (this->img_height);
}
