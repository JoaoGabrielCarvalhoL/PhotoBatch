#include "ArgumentParser.h"
#include "Utils.h"


void ArgumentParser::RegisterFlag(const std::string& flag)
{
	if (!flag.empty()) {
		m_Flags[flag] = false;
	}
}

bool ArgumentParser::GetFlag(const std::string& flag) const
{
	if (!flag.empty()) {

		//std::map<std::string, bool>::iterator flagItem = m_Flags.find(flag);
		auto flagItem = m_Flags.find(flag);

		if (flagItem != std::end(m_Flags))
		{
			return flagItem->second;
		}
	}

	return false;
}

void ArgumentParser::Parse(int argc, char* argv[])
{
	if (argc > 1)
	{
		for (int i = 1; i < argc; i++)
		{
			std::string arg = Utils::ToLower(argv[i]);

			if (arg.length() >= 3)
			{
				if (arg[0] == '-' && arg[1] == '-')
				{
					arg = arg.substr(2);

					if (arg.find_first_of('=') != std::string::npos)
					{
						const size_t equalsSignPos = arg.find('=');
						if (equalsSignPos != std::string::npos)
						{
							std::string optionName = arg.substr(0, equalsSignPos); 
							std::string optionValue = arg.substr(equalsSignPos + 1);

							auto optionIt = m_Options.find(optionName); 
							if (optionIt != std::end(m_Options))
							{
								optionIt->second = optionValue;
							}
						}
					}
					else
					{
						auto flagItem = m_Flags.find(arg);

						if (flagItem != std::end(m_Flags))
						{
							flagItem->second = true;
						}
					}
				}
			}
		}
	}
}

void ArgumentParser::RegisterOption(const std::string& option) 
{
	if (!option.empty())
	{
		m_Options[option] = "";
	}
}

const std::string& ArgumentParser::GetOption(const std::string& option) const
{
	if (!option.empty())
	{
		auto optionIt = m_Options.find(option);
		if (optionIt != std::end(m_Options))
		{
			return optionIt->second;
		}
	}

	static std::string EmptyOption = "";
	return EmptyOption;
}

float ArgumentParser::GetOptionAsFloat(const std::string& option) const 
{
	const std::string& optionValue = GetOption(option);

	if (!optionValue.empty())
	{
		return std::stof(optionValue);
	}

	return -1;
}

int ArgumentParser::GetOptionAsInt(const std::string& option) const
{
	const std::string& optionValue = GetOption(option); 
	
	if (!optionValue.empty())
	{
		return std::stoi(optionValue);
	}

	return -1;
}

void ArgumentParser::SetHelpMessage(const std::string& helpMessage)
{
	m_HelpMessage = helpMessage;
}

const std::string& ArgumentParser::GetHelpMessage() const
{
	return m_HelpMessage;
}

bool ArgumentParser::isFlagRegistered(const std::string& flag) const
{
	if (!flag.empty()) 
	{
		return m_Flags.count(flag) == 1;
	}

	return false;
}

bool ArgumentParser::isOptionRegistered(const std::string& option) const
{
	if (!option.empty())
	{
		return m_Options.count(option) == 1;
	}

	return false;
}

