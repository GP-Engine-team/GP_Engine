/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include <unordered_set>
#include <string>

#include "Kodgen/Properties/PropertyParsingSettings.h"
#include "Kodgen/Misc/Filesystem.h"

namespace kodgen
{
	class ParsingSettings
	{
		private:
			/**
			*	Include directories of the project which must be known for accurate parsing.
			*	It basically corresponds to the list of paths specified with the -I argument
			*	when compiling.
			*/
			std::unordered_set<fs::path, PathHash>	_projectIncludeDirectories;

			/**
			*	Name of the compiler used to compile the header files being parsed.
			*	This is used to make sure the parser recognizes the included headers.
			*	As for now, supported values are "clang", "gcc" and "msvc".
			*/
			std::string								_compilerExeName				= "";

		public:
			ParsingSettings()						= default;
			ParsingSettings(ParsingSettings const&)	= default;
			ParsingSettings(ParsingSettings&&)		= default;
			~ParsingSettings()						= default;

			/** Settings used when parsing C++ entities. */
			PropertyParsingSettings					propertyParsingSettings;

			/** If set to true, will parse all C++ entities, whether they are annotated or not. */
			bool									shouldParseAllEntities			= false;

			/**
			*	Should the parsing be aborted when an error is encountered or not
			*	If set to false, errors will be collected for the whole parsing
			*/
			bool									shouldAbortParsingOnFirstError	= true;

			/**
			*	@brief	Add a project include directory to the parsing settings.
			*			If the provided path is invalid of if the path was already a project include directory, do nothing.
			*	
			*	@param directoryPath Path to the include directory.
			*
			*	@return true if the provided path was added to the project include directories, else false.
			*/
			bool	addProjectIncludeDirectory(fs::path const& directoryPath)								noexcept;

			/**
			*	@brief	Remove a project directory from the parsing settings.
			*
			*	@param directoryPath Path to the include directory.
			*/
			void	removeProjectIncludeDirectory(fs::path const& directoryPath)							noexcept;

			/**
			*	@brief Getter for _projectIncludeDirectories field.
			*	
			*	@return _projectIncludeDirectories;
			*/
			inline std::unordered_set<fs::path, PathHash> const&	getProjectIncludeDirectories()	const	noexcept;

			/**
			*	@brief Getter for _compilerExeName field.
			*	
			*	@return _compilerExeName;
			*/
			inline std::string const&								getCompilerExeName()			const	noexcept;

			/**
			*	@brief	Setter for _compilerExeName field.
			*			This will also check that the compiler is indeed available on the running computer.
			*			If the compiler is not available on the running computer, the field is not set.
			*	
			*	@return true if the compiler is valid on the running computer, else false.
			*/
			bool	setCompilerExeName(std::string const& compilerExeName)											noexcept;
	};

	#include "Kodgen/Parsing/ParsingSettings.inl"
}