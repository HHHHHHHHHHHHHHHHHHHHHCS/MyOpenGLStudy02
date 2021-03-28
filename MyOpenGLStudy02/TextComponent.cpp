#include "TextComponent.h"

TextComponent::TextComponent(const char* str)
{
}

TextComponent::~TextComponent()
{
}

void TextComponent::SetText(const char* str)
{
	textContext = str;
}
