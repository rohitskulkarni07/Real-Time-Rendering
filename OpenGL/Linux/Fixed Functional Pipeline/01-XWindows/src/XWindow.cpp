#include <iostream>	// Input-Output Stream
#include <stdio.h>	// For C Standard Input-Output Function
#include <stdlib.h>	// For C Standard library Functions ex. exit()
#include <memory.h> 	// Memset

#include <X11/Xlib.h>	// XLib Releted API
#include <X11/Xutil.h>	// XVisualInfo Releted Function
#include <X11/XKBlib.h> // Releted To Keyboard symbol conversion
#include <X11/keysym.h> // KeySym Releted API

// namespaces
using namespace std;    

bool bFullscreen = false;	

/*
 struct Display{};
 It represents Graphics Device Driver And XServer,
 It is a Interface Between XClien Application And XServer 
 It Has 77 Members
 */

Display *gpDisplay = NULL;		// Pointer To Display Struct
XVisualInfo *gpXVisualInfo = NULL;	// pointer To XVisualInfo Struct, Information Releted to Visual Information screen, depth, color
Colormap gColormap;
Window gWindow;

// Window Height And Width 
int giWindowWidth = 800;
int giWindowHeight = 600;

// Entry-point Function
int main(void) {

	// Function Prototype
	void CreateWindow(void);		
	void ToggleFullscreen(void);
	void uninitialize(void);
	
	// Variable Declarations
	int winWidth = giWindowWidth;
	int winHeight = giWindowHeight;
	

	// Code
	CreateWindow();

	// Message Loop
	XEvent event;	// XEvent is Union Having Many Members out of them 33 are Struct Members 
	KeySym keysym;	// To Store The Key-symbol
	
	while(1) {
		
		/* Syntax:
			int XNextEvent(Display *display, XEvent *event_return);
		 @params
			display		:Specifies the connection to the X server.
			event_return	:returns the matched event’s associated structure.
			
		 Descpription:
			Gives Next Event In Queue
		*/
		
		XNextEvent(gpDisplay, &event);
		
		switch(event.type) {
			
			case MapNotify:		// similar to WM_CREATE
				break;
				
			case KeyPress:
				/*
				 Syntax:
					KeySym XkbKeycodeToKeysym(Display *display, KeyCode kc, unsigned int group, unsigned int level);
					
				 @params:
					display	:Specifies the connection to the X server
					kc	:key of interest
					group	:group of interest
					level	:shift level of interest
					
				 Description:
					XkbKeycodeToKeysym returns the keysym bound to a particular group and shift level for a particular key on the core keyboard. If kc is not a legal keycode for the core keyboard, or if group or level are out of range for the specified key, XkbKeycodeToKeysym returns NoSymbol.
				 
				 */
				keysym = XkbKeycodeToKeysym(gpDisplay, event.xkey.keycode, 0, 0);
				
				switch(keysym) {
					
					case XK_Escape: // Key symbol for Escape Key 
					
						uninitialize();
						exit(0);
						
					case XK_F:	// Key-Symbol for 'F' Key
					case XK_f:	// Key-Symbol for 'f' Key
						
						if(bFullscreen == false) {
							ToggleFullscreen();
							bFullscreen = true;
						}
						
						else {
							ToggleFullscreen();
							bFullscreen = false;
						}
						
						break;
						
					default:
						
						break;
				}
				
				break;

			case ButtonPress:
				switch(event.xbutton.button) {
					case 1://left mouse button
						break;
					case 2://middle mouse button
						break;
					case 3://right mouse button
						break;
					case 4://middle wheel up mouse button
						break;
					case 5://middle wheel down mouse button
						break;
					default:
						break;
				}
				break;

			case MotionNotify: 	// pointer move / curosor move
				break;
				
			case ConfigureNotify: 	// similar to WM_SIZE releted StrucutreNotifyMask
				
				winWidth = event.xconfigure.width;
				winHeight = event.xconfigure.height;
				
				break;
				
			case Expose:		// similar to WM_PAINT
				break;
				
			case DestroyNotify: 	// similar to WM_DESTROY
				break;
				
			case 33: 		// similar to DestroyNotify For dependent on Window Manager
				uninitialize();
				exit(0);
				
			default:
				break;
		}
		
	}

	uninitialize();
	
	return(0);

}

void CreateWindow(void) {
	
	// Function Prototypes
	void uninitialize(void);

	// Variable Declarations
	XSetWindowAttributes winAttribs; //Object To struct which Holds Information about Window 
	int defaultScreen;
	int defaultDepth;
	int styleMask;

	// Code
	/* 
	Syntax:
		Display *XOpenDisplay(char *display_name);
		
	@Param:
		display	:Specifies the connection to the X server.
		display_name :	Specifies the hardware display name, which determines the display and communications domain to be used.On a POSIX-conformant system, if the display_name is NULL, it defaults to the value of the DISPLAY environment variable.
		
	Descpription:	
	
		The XOpenDisplay function returns a Display structure that serves as the connection to the X server and that contains all the information about that X server.
	*/
	
	gpDisplay = XOpenDisplay(NULL);
	
	// Error-Checking
	if(gpDisplay == NULL) {
		printf("Error : Unable To Open X Display.\nExitting Now...\n");
		uninitialize();
		exit(1);	// Exit Status:	0 successfull, 1 abortive,  -1 errornous
	}
	
	// To Get Default Screen/Current Screen Connected To Current Graphic Card
	defaultScreen = XDefaultScreen(gpDisplay);
	
	// To Get Current Color Depth for Default Screen Connected to Current GPU
	defaultDepth = DefaultDepth(gpDisplay, defaultScreen);
	
	// Allocating Memory To gpXVisualInfo 
	gpXVisualInfo = (XVisualInfo *)malloc(sizeof(XVisualInfo));

	// Error-Checking
	if(gpXVisualInfo == NULL) {
		printf("Error : Unable To Allocate Memory For Visual Information.\nExiting Now...\n");
		uninitialize();
		exit(1);
	}

	/*
	Syntax:
		Status XMatchVisualInfo(Display *display, int screen, int depth, int class, XVisualInfo *vinfo_return); 
		
	@param:
		display		:Specifies the connection to the X server.
		screen		:Specifies the screen.
		depth		:Specifies the depth of the screen.
		class		:Specifies the class of the screen.
		vinfo_return	:Returns the matched visual information. 
		
	Description:
		The XMatchVisualInfo function returns the visual information for a visual that matches the specified depth and 
		class for a screen.
	*/
	
	XMatchVisualInfo(gpDisplay, defaultScreen, defaultDepth, TrueColor, gpXVisualInfo);
	
	/* Status status = XMatchVisualInfo(gpDisplay, defaultScreen, defaultDepth, TrueColor, gpXVisualInfo);
	
	if(status == 0) {
		printf("Error : Unable To Get A Visual.\nExiting Now...\n");
		uninitialize();
		exit(1);
	}
	*/
	
	//Error-Checking
	
	if(gpXVisualInfo == NULL) {
		printf("Error : Unable To Get A Visual.\nExiting Now...\n");
		uninitialize();
		exit(1);
	}
	
	/*
	
	typedef struct {
	
		Pixmap background_pixmap;	// background, None, or ParentRelative 
		unsigned long background_pixel;	// background pixel 
		Pixmap border_pixmap;		// border of the window or CopyFromParent 
		unsigned long border_pixel;	// border pixel value 
		int bit_gravity;		// one of bit gravity values 
		int win_gravity;		// one of the window gravity values 
		int backing_store;		// NotUseful, WhenMapped, Always 
		unsigned long backing_planes;	// planes to be preserved if possible 
		unsigned long backing_pixel;	// value to use in restoring planes 
		Bool save_under;		// should bits under be saved? (popups) 
		long event_mask;		// set of events that should be saved 
		long do_not_propagate_mask;	// set of events that should not propagate 
		Bool override_redirect;		// boolean value for override_redirect 
		Colormap colormap;		// color map to be associated with window 
		Cursor cursor;			// cursor to be displayed (or None)
		
	} XSetWindowAttributes;
 
	*/
	
	winAttribs.border_pixel = 0;		// Defualt Border Color
	winAttribs.background_pixmap = 0;	// No background Image
	winAttribs.background_pixel = BlackPixel(gpDisplay, defaultScreen);	// Background Color : Black

	
	/*
	Syntax:
		Colormap XCreateColormap(display, w, visual, alloc)
		
	@Param:
		display 	:Specifies the connection to the X server.
		w 		:Specifies the window on whose screen you want to create a colormap.
		visual 		:Specifies a visual type supported on the screen. If the visual type is not one supported by the screen,
				a BadMatch errorresults.
		alloc 		:Specifies the colormap entries to be allocated. You can pass AllocNone or AllocAll. 
	
	Description:
		The XCreateColormap() function creates a colormap of the specified visual type for the screen on which the specified 
		window resides and returns the colormap ID associated with it.
		Note that the specified window is only used to determine the screen.
	*/
	
	/*
	Syntax:
		Window RootWindow(Display *display, int screen_number); 
		
	@Param:
		display 	:Specifies the connection to the X server.
		screen_number	:Specifies the appropriate screen number on the host server. 
	
	Description:
		Returns the root window. 
	*/
	
	winAttribs.colormap = XCreateColormap(gpDisplay, RootWindow(gpDisplay, gpXVisualInfo->screen), gpXVisualInfo->visual, AllocNone);
	gColormap = winAttribs.colormap;
	
	/*
	 
	Event Masks:
	
		Clients select event reporting of most events relative to a window. To do this, pass an event mask to an Xlib event-handling function that takes an event_mask argument. The bits of the event mask are defined in X11/X.h. Each bit in the event mask maps to an event mask name, which describes the event or events you want the X server to return to a client application. 
		Which Event Should Sent To Your Client Application
		
		NoEventMask			No events wanted
		KeyPressMask			Keyboard down events wanted
		KeyReleaseMask			Keyboard up events wanted
		ButtonPressMask			Pointer button down events wanted
		ButtonReleaseMask		Pointer button up events wanted
		EnterWindowMask			Pointer window entry events wanted
		LeaveWindowMask			Pointer window leave events wanted
		PointerMotionMask		Pointer motion events wanted
		PointerMotionHintMask		Pointer motion hints wanted
		Button1MotionMask		Pointer motion while button 1 down
		Button2MotionMask		Pointer motion while button 2 down
		Button3MotionMask		Pointer motion while button 3 down
		Button4MotionMask		Pointer motion while button 4 down
		Button5MotionMask		Pointer motion while button 5 down
		ButtonMotionMask		Pointer motion while any button down
		KeymapStateMask			Keyboard state wanted at window entry and focus in
		ExposureMask			Any exposure wanted (similar WM_PAINT)
		VisibilityChangeMask		Any change in visibility wanted
		StructureNotifyMask		Any change in window structure wanted
		ResizeRedirectMask		Redirect resize of this window
		SubstructureNotifyMask		Substructure notification wanted
		SubstructureRedirectMask	Redirect structure requests on children
		FocusChangeMask			Any change in input focus wanted
		PropertyChangeMask		Any change in property wanted
		ColormapChangeMask		Any change in colormap wanted
		OwnerGrabButtonMask		Automatic grabs should activate with owner_events set to True
	*/
	
	winAttribs.event_mask = ExposureMask | VisibilityChangeMask | ButtonPressMask | KeyPressMask | PointerMotionMask | StructureNotifyMask;

	/*
	Style Masks:
	
		What Style Given To Window
		
		CWBackPixel 	Background Pixel Wanted
		CWBorderPixel 	Border Wanted
		CWColormap 	Color Map Wanted
		CWEventMask 	Evenet Mask Wanted
		
	*/
	styleMask = CWBorderPixel | CWBackPixel | CWEventMask | CWColormap;
	
	
	/*
	Syntax:
		Window XCreateWindow(display, parent, x, y, width, height, border_width, depth, class, visual, valuemask, attributes)
	@Param: 
		display 	:Specifies the connection to the X server.
		parent 		:Specifies the parent window.
		x, y 		:Specify the x and y coordinates, which are the top-left outside corner of the window's borders and are relative to the inside of the parent window's borders
		width,height 	:Specify the width and height, which are the created window's inside dimensions and do not include the created  window'borders.
		border_width	:Specifies the width of the created window's border in pixels.
		depth 		:Specifies the window's depth. A depth of CopyFromParent means the depth is taken from the parent.
		class		:Specifies the created window's class. You can pass InputOutput, InputOnly, or CopyFromParent. A class of CopyFromParent means the class is taken from the parent.
		visual		:Specifies the visual type. A visual of CopyFromParent means the visual type is taken from the parent.
		valuemask 	:Specifies which window attributes are defined in the attributes argument.
				 This mask is the bitwise inclusive OR of the valid attribute mask bits. 
				 If valuemask is zero, the attributes are ignored and are not referenced.
		attributes	:Specifies the structure from which the values (as specified by the value mask) are to be taken. The value mask should  have the appropriate bits set to indicate which attributes have been set in the structure.
		
		
	Description:
		The XCreateWindow function creates an unmapped subwindow for a specified parent window, returns the window ID of the created window, and causes the X server to generate a CreateNotify event. The created window is placed on top in the stacking order with respect to siblings. 
	*/
	
	gWindow = XCreateWindow(gpDisplay,
				RootWindow(gpDisplay, gpXVisualInfo->screen),
				0,
				0,
				giWindowWidth,
				giWindowHeight,
				0,
				gpXVisualInfo->depth,
				InputOutput,
				gpXVisualInfo->visual,
				styleMask,
				&winAttribs);
	
	if(!gWindow) {
		printf("Error : Failed To Create Main Window.\nExiting Now...\n");
		uninitialize();
		exit(1);
	}

	// Caption To Window
	XStoreName(gpDisplay,gWindow, "RTR2020-004 Rohit Sudhir Kulkarni");
	
	
	// To Close The Window Its Not Need OF XWindow(Windowing API) But Its Need Of Window Manager So Window Manger has its Own API
	// Following Funtion Which Starts With Pre-Fix WM Which Means Window Manager
	// To Give Order To Window Manager Which Is XSetWMProtocols Available In Xprotocols
	// Atom Is Nothing But Immutable-Unique String
	// Number 33 represents WM_DELETE_WINDOW Message or Event This Is Done For Archieveing N/W Complience
	
	
	/*
	 Syntax:
	 
		Atom XInternAtom(Display display,char atom_name,bool only_if_exists);
	
	@params:
		display		:Specifies the connection to the X server.
		atom_name	:Specifies the name associated with the atom you want returned.
		only_if_exists	:Specifies a Boolean value that indicates whether the atom must be created.
		
	Description:
	
		To Create Atom, This Function is Used,The XInternAtom() function returns the atom identifier associated with the specified atom_name string. If only_if_exists is False, the atom is created if it does not exist. Therefore, XInternAtom() can return None. If the atom name is not in the Host Portable Character Encoding, the result is implementation dependent. Uppercase and lowercase matter; the strings ``thing'', ``Thing'', and ``thinG'' all designate different atoms. The atom will remain defined even after the client's connection closes. It will become undefined only when the last connection to the X server closes.
	*/
	
	Atom windowManagerDelete = XInternAtom(gpDisplay, "WM_DELETE_WINDOW", True);
	
	/*
	Syntax:
		 XSetWMProtocols(Display *display,Window w,Atom *protocols,int count);
		 
	@params:
		
		display		:Specifies the connection to the X server.
		w		:Specifies the window.
		protocols	:Specifies the list of protocols.
		count		:Specifies the number of protocols in the list.

	Description:
	
		The XSetWMProtocols() function replaces the WM_PROTOCOLS property on the specified window with the list of atoms specified by the protocols argument. If the property does not already exist, XSetWMProtocols() sets the WM_PROTOCOLS property on the specified window to the list of atoms specified by the protocols argument. The property is stored with a type of ATOM and a format of 32. If it cannot intern the WM_PROTOCOLS atom, XSetWMProtocols() returns a zero status. Otherwise, it returns a nonzero status.
	*/
	
	XSetWMProtocols(gpDisplay, gWindow, &windowManagerDelete,1);

	//Show Window On Display
	XMapWindow(gpDisplay, gWindow);

}


void ToggleFullscreen(void) {

	// Variable Declarations
	Atom wm_state; 		// Window's Current State
	Atom fullscreen;	// fullscreen state's atom
	XEvent xev={0};		// Create Object Of Event And Make All Of Members Zero

	// Code
	
	wm_state = XInternAtom(gpDisplay, "_NET_WM_STATE", False); 	// _NET_WM_STATE Currnet Window State Accros The Network

	memset(&xev, 0, sizeof(xev)); 	// use memset to zero out or initalise all to zero use either one 

	xev.type = ClientMessage;			// Type Of Message of Client (Send To Client)
	xev.xclient.window = gWindow;			// To Whcih Window to send message
	xev.xclient.message_type = wm_state;		// What Order to send
	xev.xclient.format = 32;			// Order Byte 32 bit message
	xev.xclient.data.l[0] = bFullscreen ? 0 : 1;	// Toggle The State fullscreen to normal or normal to fullscreen
	
	fullscreen = XInternAtom(gpDisplay, "_NET_WM_STATE_FULLSCREEN", False);
	xev.xclient.data.l[1] = fullscreen;

	XSendEvent(gpDisplay, RootWindow(gpDisplay, gpXVisualInfo->screen),False,StructureNotifyMask,&xev);

}



void uninitialize(void) {
	
	
	if(gWindow) {
		// unallocate memory given to window
		XDestroyWindow(gpDisplay, gWindow);
	
	}
	
	if(gColormap) {
		
		// Free Color-Map
		XFreeColormap(gpDisplay,gColormap);
	}
	
	if(gpXVisualInfo) {
		// Free gpXVisualInfo
		free(gpXVisualInfo);
		gpXVisualInfo = NULL;
	}
	
	if(gpDisplay) {
		// Dissconect the Display To Client App
		XCloseDisplay(gpDisplay);
		gpDisplay = NULL;
	}
}


	
