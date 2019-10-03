#pragma once

#include <string>
#include "ModelElementBuffer.h"

class FileInputOutput
{
	ModelElementBuffer* M;

public:
	FileInputOutput(ModelElementBuffer*);
	~FileInputOutput();

	void exportTextFile(std::string);
};

