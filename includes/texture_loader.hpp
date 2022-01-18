#ifndef TEXTURE_LOADER_HPP
#define TEXTURE_LOADER_HPP

#include <fstream>
#include <exception>
#include "String.hpp"

namespace Scop
{
	class TextureLoader
	{
	private:
		ft::String		path;
		unsigned char	*file_content;
		size_t			content_size;

		size_t			img_width;
		size_t			img_height;
		unsigned char	*img;
		size_t			img_size;

		TextureLoader();
	public:
		TextureLoader(ft::String path_to_bmp);
		~TextureLoader();
		TextureLoader(TextureLoader &rhs);
		TextureLoader operator=(TextureLoader &rhs);

		void readFile(ft::String path);
		void parseFile();

		unsigned char* getPixelArray();
		size_t getWidth();
		size_t getHeight();
	};

	class TextureLoaderException : public std::exception
	{
		private:
			ft::String message;

		public:
			TextureLoaderException(ft::String message) {
				message = message;
			};
			~TextureLoaderException() throw() {};

			const char* what() const throw() {
				return message.c_str();
			};
	};

}

#endif
