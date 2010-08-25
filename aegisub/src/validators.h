// Copyright (c) 2005, Rodrigo Braz Monteiro
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//   * Neither the name of the Aegisub Group nor the names of its contributors
//     may be used to endorse or promote products derived from this software
//     without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Aegisub Project http://www.aegisub.org/
//
// $Id$

/// @file validators.h
/// @see validators.cpp
/// @ingroup custom_control utility
///




///////////
// Headers
#ifndef AGI_PRE
#include <wx/validate.h>
#endif


/// DOCME
/// @class NumValidator
/// @brief DOCME
///
/// DOCME
class NumValidator : public wxValidator {
private:

	/// DOCME
	double fValue;

	/// DOCME
	int iValue;

	/// DOCME
	wxString* valPtr;


	/// DOCME
	bool isFloat;

	/// DOCME
	bool isSigned;
	wxObject* Clone() const;
	bool Validate(wxWindow* parent);
	bool TransferToWindow();
	bool TransferFromWindow();

	bool CheckCharacter(int chr,bool isFirst,bool canSign,bool &gotDecimal);

	void OnChar(wxKeyEvent& event);

public:
	NumValidator(wxString* valPtr = NULL,bool isfloat=false,bool issigned=false);
	NumValidator(const NumValidator& from);

	DECLARE_EVENT_TABLE();
};