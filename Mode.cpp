#include "Mode.h"
#include "ArgumentParser.h"
#include "RenameMode.h"
#include "ConvertMode.h"
#include "ResizeMode.h"
#include "ScaleMode.h"

#include <array>
#include <filesystem>
#include <chrono>
#include <iostream>



Mode::Mode(const std::string& filter, const std::string& folder)
{
	m_Filter = filter;
	m_Folder = folder;
}


const std::string& Mode::GetFilter() const
{
	return m_Filter;
}

const std::string& Mode::GetFolder() const
{
	return m_Folder;
}

void Mode::Run()
{
	std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();
	RunImpl();
	std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();

	std::chrono::high_resolution_clock::duration elapsedTime = endTime - startTime;
	std::chrono::milliseconds elapsedTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime);
	 
	std::cout << GetModeName() << "Operação finalizada em: " << elapsedTimeMs.count() << " ms" << std::endl;
}

std::vector<std::filesystem::path> Mode::GetFiles(const std::filesystem::path& extension) const
{
	std::vector<std::filesystem::path> files;

	int numSkippedFiles = 0;
	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(GetFolder()))
	{
		//std::cout << entry << std::endl;
		const bool bIsFile = std::filesystem::is_regular_file(entry.path());
		const bool bMatchFilter = GetFilter().empty() || (entry.path().string().find(GetFilter()) != std::string::npos);
		const bool bMatchExtension = extension.empty() || (entry.path().extension() == extension);

		if (bIsFile && bMatchFilter && bMatchExtension)
		{
			files.push_back(entry.path());
		}
		else
		{
			numSkippedFiles++;
		}

	}

	std::cout << GetModeName() << "Número de arquivos encontrados: " << files.size() << std::endl;
	std::cout << GetModeName() << "Número de arquivos ignorados: " << numSkippedFiles << std::endl;

	return files;
}


const std::string& GetInvalidChars()
{
	static const std::string invalidCaracters = "\\/*:?\"<>|";
	return invalidCaracters;
}

bool HasInvalidChars(const std::string& str)
{

	const bool bHasInvalidChars = str.find_first_of(GetInvalidChars()) != std::string::npos;
	return bHasInvalidChars;

}



std::unique_ptr<Mode> CreateMode(const ArgumentParser& argParser)
{
	const bool bRenameMode = argParser.GetFlag(Args::Flags::Rename);
	const bool bConvertMode = argParser.GetFlag(Args::Flags::Convert);
	const bool bResizeMode = argParser.GetFlag(Args::Flags::Resize);
	const bool bScaleMode = argParser.GetFlag(Args::Flags::Scale);


	/* Método substituído pelo o subsequente
	*
		//Verificar se somente um modo do PhotoBatch está ativo
		if (!(bRenameMode ^ bConvertMode ^ bResizeMode ^ bScaleMode))
		{
			throw std::invalid_argument("Somente um modo pode estar ativo!");
		}
	*/

	const std::array<bool, 4> flags = { bRenameMode, bConvertMode, bResizeMode, bScaleMode };
	const std::ptrdiff_t numberActivesFlags = std::count(std::begin(flags), std::end(flags), true);

	if (numberActivesFlags != 1)
	{
		throw std::invalid_argument("Somente um modo pode estar ativo!");
	}

	const std::string folder = argParser.GetOptionAs <std::string>(Args::Opts::Folder);

	if (folder.empty())
	{
		throw std::invalid_argument("É necessário informar um diretório válido!");
	}

	if (!std::filesystem::exists(folder))
	{
		throw std::invalid_argument("O diretório informado não existe!");
	}

	const std::string filter = argParser.GetOptionAs<std::string>(Args::Opts::Filter);
	if (!filter.empty() && HasInvalidChars(filter))
	{
		throw std::invalid_argument("O filtro não pode conter: " + GetInvalidChars());
	}

	if (bResizeMode)
	{

		int width = 0;
		int height = 0;

		try
		{
			width = argParser.GetOptionAs<int>(Args::Opts::Widht);
			height = argParser.GetOptionAs<int>(Args::Opts::Height);
		}

		catch (const std::invalid_argument&)
		{
			throw std::invalid_argument("O valor informado para Width ou Height não são números válidos");
		}

		if (width <= 0 || height <= 0)
		{
			throw std::invalid_argument("Width e Heigth devem ser maiores que zero!");
		}

		if (filter.empty())
		{
			throw std::invalid_argument("Filter não pode estar em branco no modo Resize");
		}

		return std::make_unique<ResizeMode>(filter, folder, width, height);
	}

	if (bScaleMode)
	{
		float amount = 0.0f;

		try
		{
			amount = argParser.GetOptionAs<float>(Args::Opts::Amount);
		}
		catch (const std::invalid_argument&)
		{
			throw std::invalid_argument("O valor informado para Amount não é um número válido!");
		}

		if (amount <= 0.0f)
		{
			throw std::invalid_argument("Amount deve ser maior do que zero!");
		}

		if (filter.empty())
		{
			throw std::invalid_argument("Filter não pode estar em branco no modo Scale");
		}

		return std::make_unique<ScaleMode>(filter, folder, amount);
	}

	if (bRenameMode)
	{
		int startNumber = -1;

		try
		{
			startNumber = argParser.GetOptionAs<int>(Args::Opts::StartNumber);
		}
		catch (const std::invalid_argument&)
		{
			throw std::invalid_argument("O valor informado para StartNumber não é um número válido.");
		}

		std::string prefix = argParser.GetOptionAs<std::string>(Args::Opts::Prefix);

		if (startNumber < 0)
		{
			throw std::invalid_argument("StartNumber deve ser maior ou igual a zero");
		}

		if (prefix.empty() || HasInvalidChars(prefix))
		{
			throw std::invalid_argument("Prefix não pode ser vazio e nem conter os caracteres: " + GetInvalidChars());
		}

		return std::make_unique<RenameMode>(filter, folder, prefix, startNumber);
	}

	if (bConvertMode)
	{
		const std::string from = argParser.GetOptionAs<std::string>(Args::Opts::From);
		const std::string to = argParser.GetOptionAs<std::string>(Args::Opts::To);

		const std::array<std::string, 2> convertOptions = { "jpg", "png" };

		const bool bIsFromValid = std::find(std::begin(convertOptions), std::end(convertOptions), from) != std::end(convertOptions);
		const bool bIsToValid = std::find(std::begin(convertOptions), std::end(convertOptions), to) != std::end(convertOptions);

		if (!bIsFromValid || !bIsToValid)
		{
			throw std::invalid_argument("From e To devem ser jpg ou png!");
		}

		if (from == to)
		{
			throw std::invalid_argument("From e To devem ser diferentes!");
		}

		const std::map<std::string, ConvertMode::Format> convertOptionsMap =
		{
			{"jpg", ConvertMode::Format::JPG},
			{"png", ConvertMode::Format::PNG}
		};

		return std::make_unique<ConvertMode>(filter, folder, convertOptionsMap.at(from), convertOptionsMap.at(to));
	}

	return nullptr;
}