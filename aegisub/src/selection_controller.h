// Copyright (c) 2010, Niels Martin Hansen
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

/// @file selection_controller.h
/// @ingroup controllers
/// @brief Interface declaration for the SubtitleSelectionController


#include <set>


// For now, the type of subtitle lines being edited
class AssDialogue;


/// Type of a list of subtitle lines marked for potential manipulation
typedef std::vector<AssDialogue *> SubtitleSelection;


class SubtitleSelectionListener;


/// @class SubtitleSelectionController
/// @brief Abstract interface for subtitle selection controllers
///
/// Two concepts are managed by implementations of this interface: The concept of the
/// active line, and the concept of the set of selected lines. There is one or zero
/// active lines, the active line is the base for subtitle manipulation in the GUI.
/// The set of selected lines may contain any number of subtitle lines, and those
/// lines are the primary target of subtitle manipulation. In other words, the active
/// line controls what values the user is presented to modify, and the selected set
/// controls what lines are actually modified when the user performs modifications.
/// In most cases, the active line will be a member of the selected set. It will be
/// the responsibility of manipulators to affect the appropriate lines.
///
/// There is only intended to be one instance of a class implementing this interface
/// per editing session, but there may be many different implementations of it.
/// The primary implementation would be the subtitle grid in the main GUI, allowing
/// the user to actively manipulate the active and selected line sets, but other
/// potential implementations are in a test driver and in a non-interactive scenario.
///
/// Objects implementing the SubtitleSelectionListener interface can subscribe to
/// changes in the active line and the selected set.
class SubtitleSelectionController {
public:
	/// Virtual destructor for safety
	virtual ~SubtitleSelectionController() { }

	/// @brief Change the active line
	/// @param new_line Subtitle line to become the new active line
	///
	/// The active line may be changed to NULL, in which case there is no longer an
	/// active line.
	///
	/// Calling this method should only cause a change notification to be sent if
	/// the active line was actually changed.
	virtual void SetActiveLine(AssDialogue *new_line) = 0;
	
	/// @brief Obtain the active line
	/// @return The active line or NULL if there is none
	virtual AssDialogue * GetActiveLine() const = 0;

	/// @brief Change the selected set
	/// @param new_selection The set of subtitle lines to become the new selected set
	///
	/// Implementations must either completely change the selected set to the new
	/// set provided, or not change the selected set at all. Partial changes are
	/// not allowed.
	///
	/// If no change happens to the selected set, whether because it was refused or
	/// because the new set was identical to the old set, no change notification may
	/// be sent.
	virtual void SetSelectedSet(const SubtitleSelection &new_selection) = 0;

	/// @brief Obtain the selected set
	/// @param[out] selection Filled with the selected set on return
	virtual void GetSelectedSet(SubtitleSelection &selection) const = 0;

	/// @brief Change the active line to the next in sequence
	///
	/// If there is no logical next line in sequence, no change happens. This should
	/// also reset the selected set to consist of exactly the active line, if the
	/// active line was changed.
	virtual void NextLine() = 0;

	/// @brief Change the active line to the previous in sequence
	///
	/// If there is no logical previous line in sequence, no change happens. This
	/// should also reset the selected set to consist of exactly the active line, if
	/// the active line was changed.
	virtual void PrevLine() = 0;

	/// @brief Subscribe an object to receive change notifications
	/// @param listener Object to subscribe to change notifications
	virtual void AddSelectionListener(SubtitleSelectionListener *listener) = 0;

	/// @brief Unsubscribe an object from change notifications
	/// @param listener Object to unsubscribe from change notifications
	virtual void RemoveSelectionListener(SubtitleSelectionListener *listener) = 0;
};


/// @class SubtitleSelectionListener
/// @brief Abstract interface for classes wanting to subtitle selection change notifications
class SubtitleSelectionListener {
public:
	/// Virtual destructor for safety
	virtual ~SubtitleSelectionListener() { }

	/// @brief Called when the active subtitle line changes
	/// @param new_line The subtitle line that is now the active line
	virtual void OnActiveLineChanged(AssDialogue *new_line) = 0;

	/// @brief Called when the selected set changes
	/// @param new_selection The subtitle line set that is now the selected set
	virtual void OnSelectedSetChanged(const SubtitleSelection &new_selection) = 0;
};



/// @class BaseSubtitleSelectionController
/// @brief Base-implementation of SubtitleSelectionController
///
/// This class implements adding and removing listeners for selection change
/// notifications, and provides protected functions to announce selection changes.
///
/// This class should be derived from for most real-world uses, but might not
/// be desirable in some special cases such as test drivers.
class BaseSubtitleSelectionController : public SubtitleSelectionController {
	std::set<SubtitleSelectionListener *> listeners;

protected:
	/// Call OnActiveLineChanged on all listeners
	void AnnounceActiveLineChanged(AssDialogue *new_line)
	{
		for (std::set<SubtitleSelectionListener *>::iterator listener = listeners.begin(); listener != listeners.end(); ++listener)
		{
			(*listener)->OnActiveLineChanged(new_line);
		}
	}

	/// Call OnSelectedSetChangedon all listeners
	void AnnounceSelectedSetChanged(const SubtitleSelection &new_selection)
	{
		for (std::set<SubtitleSelectionListener *>::iterator listener = listeners.begin(); listener != listeners.end(); ++listener)
		{
			(*listener)->OnSelectedSetChanged(new_selection);
		}
	}

public:
	virtual void AddSelectionListener(SubtitleSelectionListener *listener)
	{
		listeners.insert(listener);
	}

	virtual void RemoveSelectionListener(SubtitleSelectionListener *listener)
	{
		listeners.erase(listener);
	}
};



/// Do-nothing selection controller, can be considered to always operate on an empty subtitle file
class DummySubtitleSelectionController : public SubtitleSelectionController {
public:
	virtual void SetActiveLine(AssDialogue *new_line) { }
	virtual AssDialogue * GetActiveLine() const { return 0; }
	virtual void SetSelectedSet(const SubtitleSelection &new_selection) { }
	virtual void GetSelectedSet(SubtitleSelection &selection) const { }
	virtual void NextLine() { }
	virtual void PrevLine() { }
	virtual void AddSelectionListener(SubtitleSelectionListener *listener) { }
	virtual void RemoveSelectionListener(SubtitleSelectionListener *listener) { }
};