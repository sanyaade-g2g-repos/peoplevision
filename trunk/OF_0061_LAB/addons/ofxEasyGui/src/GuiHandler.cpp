/*---------------------------------------------------
Open Frameworks 0.05
graphical user interface - Dennis Rosenfeld-2008
*///--------------------------------
#include "GuiHandler.h"


GuiHandler::GuiHandler()//constructor
{
    initialize();
}


GuiHandler::~GuiHandler()//destructor
{
}


void    GuiHandler::initialize()
{

    isHidden = false;
    hideKey = 'h';
    keysWhileHidden = true;

    isTypingBlocked = false;
	
	ofAddListener(ofEvents.mouseMoved, this, &GuiHandler::mouseMoved);
	ofAddListener(ofEvents.mousePressed, this, &GuiHandler::mousePressed);
	ofAddListener(ofEvents.mousePressed, this, &GuiHandler::mousePressed);
	ofAddListener(ofEvents.mouseDragged, this, &GuiHandler::mouseDragged);
	ofAddListener(ofEvents.mouseReleased, this, &GuiHandler::mouseReleased);	
	ofAddListener(ofEvents.keyPressed,this,&GuiHandler::keyPressed);
	ofAddListener(ofEvents.keyReleased,this,&GuiHandler::keyReleased);
	
//    ofMouseEvents.addListener(this);
//	ofKeyEvents.addListener(this);
	//ofAppEvents.addListener(this);
    //PanelList = new GuiPanel[5];
}

GuiPanel*   GuiHandler::addPanel(string name,int x,int y,int w, int h)
{
    GuiPanel* newPanel = new GuiPanel;
    newPanel->initialize(name,x,y,w,h);
    panelList.push_back(newPanel);

    return newPanel;
}

bool    GuiHandler::updateBool(int idNum)
{
    bool tempVal;

    for (int i = 0; i < panelList.size(); i++ )
    {
        for (int j = 0; j < panelList[i]->elements.size(); j++ )
        {
            if ( idNum == panelList[i]->elements[j]->getID() )
            {
                tempVal = panelList[i]->elements[j]->getValueBool();
            }
        }
    }
    return tempVal;
}


float    GuiHandler::updateFloat(int idNum)
{
    float tempVal;

    for (int i = 0; i < panelList.size(); i++ )
    {
        for (int j = 0; j < panelList[i]->elements.size(); j++ )
        {
            if ( idNum == panelList[i]->elements[j]->getID() )
            {
                tempVal = panelList[i]->elements[j]->getValueFloat();
            }
        }
    }
    return tempVal;
}

string    GuiHandler::updateString(int idNum)
{
    string tempVal;

    for (int i = 0; i < panelList.size(); i++ )
    {
        for (int j = 0; j < panelList[i]->elements.size(); j++ )
        {
            if ( idNum == panelList[i]->elements[j]->getID() )
            {
                tempVal = panelList[i]->elements[j]->getValueString();
            }
        }
    }
    return tempVal;
}

int    GuiHandler::updateInt(int idNum)
{
    int tempVal;

    for (int i = 0; i < panelList.size(); i++ )
    {
        for (int j = 0; j < panelList[i]->elements.size(); j++ )
        {
            if ( idNum == panelList[i]->elements[j]->getID() )
            {
                tempVal = panelList[i]->elements[j]->getValueInt();
            }
        }
    }
    return tempVal;
}

void    GuiHandler::draw()
{
    if (isHidden == false)
    {
        for (int i = 0; i < panelList.size(); i++ )
        {
            panelList[i]->draw();
        }
    }
}

void    GuiHandler::hide()
{
    isHidden = true;
    isTypingBlocked = true;

}

void    GuiHandler::unHide()
{
    isHidden = false;
    isTypingBlocked = false;
}

void    GuiHandler::mouseMoved(ofMouseEventArgs& _args)
{
	int x = _args.x;
	int y = _args.y;
    if (isHidden == false)
    {
        for (int i = 0; i < panelList.size(); i++ )
        {
          if ( panelList[i]->checkBoundaries(x,y) == true)
          {
            panelList[i]->setLine(true);
            panelList[i]->mouseMoved(x,y);
			  updated = true;
          }
          else
            {
            panelList[i]->setLine(false);
          }
        }
    }
}

void    GuiHandler::mousePressed(ofMouseEventArgs& _args)
{
	int x = _args.x;
	int y = _args.y;
	
	cout<<"MOUSE PRESSED AT "<<x<<","<<y<<endl;
	
	int button = _args.button;
    if (isHidden == false)
    {
        for (int i = 0; i < panelList.size(); i++ )
        {
          if ( panelList[i]->checkBoundaries(x,y) == true)
          {
			  panelList[i]->mousePressed(x,y,button);
			  updated = true;
          }

          else //not in boundaries.
          {
            //check elements for textfields and depress them.
            for (int j = 0; j < panelList[i]->elements.size(); j++ )
            {
                if ( panelList[i]->elements[j]->checkType() == "TEXTFIELD")
                {
                   panelList[i]->elements[j]->dePress();
                }
            }
          }

         }//end panel size.
    }
}

void    GuiHandler::mouseReleased(ofMouseEventArgs& _args)
{
    if (isHidden == false)
    {
        for (int i = 0; i < panelList.size(); i++ )
        {
            panelList[i]->mouseReleased();
			updated = true;
        }
    }
}


void    GuiHandler::mouseDragged(ofMouseEventArgs& _args)
{
	int x = _args.x;
	int y = _args.y;
	int button = _args.button;
    if (isHidden == false)
    {
        for (int i = 0; i < panelList.size(); i++ )
        {
          if ( panelList[i]->checkBoundaries(x,y) == true)
          {
			  panelList[i]->mouseDragged(x,y,button);
			  updated = true;
          }

        }
    }
}


void GuiHandler::keyPressed(ofKeyEventArgs& _args)
{
	int key = _args.key;

    bool keypressOK = true;
    if (isTypingBlocked == false)
    {
            for (int i = 0; i < panelList.size(); i++ )
            {
                for (int j = 0; j < panelList[i]->elements.size(); j++ )
                {

                    if ( panelList[i]->elements[j]->checkType() == "TEXTFIELD")
                    {
                        if( panelList[i]->elements[j]->keyPressOK() == false)
                        {
                            keypressOK = false;
                            panelList[i]->elements[j]->keyPressed(key);//only pass keys to the textbox.
							updated = true;
                        }
                    }
                }
            }
            if (keypressOK == true)
            {
                if (key == hideKey)
                {
                    if ( isHidden == false)
                    {
                     isHidden = true;
                    }
                    else if ( isHidden == true)
                    {
                     isHidden = false;
                    }

                }

                if (isHidden == false || keysWhileHidden == true)
                {
                    for (int i = 0; i < panelList.size(); i++ )
                    {
                        for (int j = 0; j < panelList[i]->elements.size(); j++ )
                        {
                            panelList[i]->elements[j]->keyPressed(key);
							updated = true;
                            //cout<<"typing test";
                        }
                    }
                }
            }
    }
}

void GuiHandler::keyReleased(ofKeyEventArgs& _args)
{
	int key = _args.key;
    if (isHidden == false)
    {
        for (int i = 0; i < panelList.size(); i++ )
        {
            for (int j = 0; j < panelList[i]->elements.size(); j++ )
            {
				panelList[i]->elements[j]->keyReleased(key);
				updated = true;
            }
        }
    }
}

void GuiHandler::setHideKey(int key)
{
    hideKey = key;
}


bool    GuiHandler::listenForTrigger(int idNum)
{
    int tempVal;

    for (int i = 0; i < panelList.size(); i++ )
    {
        for (int j = 0; j < panelList[i]->elements.size(); j++ )
        {
            if ( idNum == panelList[i]->elements[j]->getID() )
            {
                tempVal = panelList[i]->elements[j]->listenForTrigger();
            }
        }
    }
    return tempVal;
}


bool   GuiHandler::isSafeToType()
{
    bool keypressOK = true;
    for (int i = 0; i < panelList.size(); i++ )
    {
        for (int j = 0; j < panelList[i]->elements.size(); j++ )
        {
            if ( panelList[i]->elements[j]->checkType() == "TEXTFIELD")
            {
                if( panelList[i]->elements[j]->keyPressOK() == false)
                {
                    keypressOK = false;
                }
            }
        }
    }
    return keypressOK;
}

bool      GuiHandler::isGuiHidden()
{
 return isHidden;
}



void        GuiHandler::disableKeys()
{
    isTypingBlocked = true;
}
void        GuiHandler::enableKeys()
{
    isTypingBlocked = false;
}

//------------XML JAMZ ----------------------------------------------------------
//-------------------------------------------------------------------------------
//note: this doesn't work.

void GuiHandler::loadFromXML( string file)
{
	xmlFile = file;
	
	if(XML.loadFile(file)) {
		
		//loop through each panel
		
		int numPanels = XML.getNumTags("panel");
		
		for (int i=0; i<numPanels; i++){
			
			// ---  BUILD PANEL ---
			
			XML.pushTag("panel");
			
			//get + set panel attributes
			string	panelName = XML.getValue("element:name", "panel", 0);
			int		x = XML.getValue("element:x", 0, 0);
			int		y = XML.getValue("element:y", 0, 0);
			int		w = XML.getValue("element:w", 0, 0);
			int		h = XML.getValue("element:h", 0, 0);
			
			GuiPanel* panel = addPanel(panelName, x, y, w, h);
			
			// --- ADD ELEMENTS ---
			
			int numElements = XML.getNumTags("elements");
			
			if(numElements > 0) {
				XML.pushTag("elements", numElements-1);

				//loop through each element in order
				int numElements  = XML.getNumTags("element");
				for(int j=0; j<numElements; j++) {
					
					string type		= XML.getValue("element:type", "", j);
					
					XML.pushTag("element", j);
					
					if (type == "button"){
						
						//panel->addButton(XML.getValue("name", "name", 0), XML.getValue("w", 10, 0), XML.getValue("h", 10, 0), XML.getValue("switchmode", "", 0), XML.getValue("textontop", false, 0), XML.getValue("id", 0, 0), &());
					} else if (type == "filelist"){
					} else if (type == "meter"){
					} else if (type == "radio"){
					} else if (type == "slider"){
					} else if (type == "spacer"){
					} else if (type == "textfield"){
					}
					
					XML.popTag();
				}
				XML.popTag();
			}	
			
			
			XML.popTag();
		}
		
		cout << " --- GUI set From XML ---\n" <<endl;
	} else {
		cout << " --- ERROR IN XML --- "<<endl;
	}
	
};


void GuiHandler::saveToXML( string file)
{
	
};

