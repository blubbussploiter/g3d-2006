#ifndef CONTENT_H
#define CONTENT_H

#include <string>

namespace RBX
{
	enum MimeTypes
	{
		Mesh,
		Png,
	};

	class Content
	{
	public:
		bool isBinary;
		bool isEmpty;

		std::string binary;
		std::string contentUrl;
	public:
		Content(std::string content) : isBinary(0), contentUrl(content) {}
		Content(bool isBinary, std::string binary) : isBinary(isBinary), binary(binary) {}
		Content(bool isBinary) : isBinary(isBinary) {}
		Content() : isBinary(0), isEmpty(1) {}
	};

	class ContentProvider
	{
	public:
		void downloadContent(Content content, std::string& contentPath);
		void cleanupContent(Content content);
		static ContentProvider* singleton();
	};
}

#endif