#pragma once

//-------------------------------------------------------
//
//  PbaThing.h
//
//  Generic container for things that work
//  in cooperation with the PbaViewer
//
//  Copyright (c) 2017 Jerry Tessendorf
//
//
//--------------------------------------------------------


#include <string>
#include <vector>
#include <memory>

#include "../../Object.h"

using namespace std;

namespace pba {

	class PbaThingyDingy : public pba::Object
	{
	public:
		PbaThingyDingy(const std::string& nam = "PbaThingyDingyNoName");
		virtual ~PbaThingyDingy() {};

		virtual void Init(const std::vector<std::string>& args) {};

		//
		//  Graphics interface
		//

		// Callback functions
		//! Cascading callback for initiating a display event
		virtual void Display() {};
		//! Cascading callback for a keyboard event 
		virtual void Keyboard(int key, int scancode, int action, int mods);
		//! Cascading callback for a mouse event 
		virtual void Mouse(int button, int action, int mods) {};
		//! Cascading callback for a mouse motion event 
		virtual void Motion(int x, int y) {};
		//! Cascading callback for a GLUT Special event 
		virtual void Special(int key, int x, int y) {};
		//! Cascading callback for an idle  event 
		virtual void Idle();
		//! Cascading callback for a window reshape 
		virtual void Reshape(int w, int h) {};
		//! Cascading callback for reseting parameters
		virtual void Reset() {};
		//! Cascading callback to home parameters
		virtual void Home() {};
		//! Cascading callback for usage information
		virtual void Usage();

		//! Provides the name assigned to the thingy dingy
		const std::string& Name() const { return name; }

		bool isVisible() const { return visible; }
		void setInvisible() { visible = false; }
		void setVisible() { visible = true; }
		void toggleVisible() { visible = !visible; }
		void toggleAnimate() { animate = !animate; }

		//
		//  Dynamics interface
		//

		//! Advance dynamical data by one time step
		virtual void solve() {};
		//! Set the length of time for a time step
		void SetSimulationTimestep(const double v) { dt = v; }

	protected:
		bool visible;
		double dt;
		const std::string name;

	private:
		bool animate;
		PbaThingyDingy() {};
	};

	typedef std::shared_ptr<PbaThingyDingy> PbaThing;
}
