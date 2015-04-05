#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glui.h>
#include <math.h>
#include <time.h>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <pthread.h>
#include <cstdlib>
#include <functional>
#include <numeric>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <sstream>
#include <string>

using namespace std ;

//========= global ========
int winwidth = 1000 , winheight = 1000 ;
float tup,tdown,tleft,tright;
double aspectx=1.0,aspecty=1.0;
int del_x = 0.0 , del_y = 0.0 ; 
double my , mx ;
//=========================


//======== Database ======



class player
{
public:
	double pos_x ;
	double pos_y ;
	
	/*double vel ;
	double angle ;
	
	double R, G, B ;
	
	double health ;

	double ammo ;
	vector<int> weaps ;*/
};

vector<pair< pair<float,float> ,pair<float,float> > > wall_coordinates;
vector<player> players ;
vector<pair<unsigned int,unsigned short> > player_ip;
//=====================





void Draw_wall(pair< pair<float,float> ,pair<float,float> > p){
	glColor4f(1.0,0.5,1.0,1.0);
	glRectf(p.first.first,p.first.second, p.second.first , p.second.second);
}

void Draw_man(player p){
	glColor4f(0.37,0.61,0.62,1.0);
	glBegin(GL_POLYGON);
	for(int i = 0 ; i < 100 ; i++)
	{
		double ang = 2.0* 3.1415926 * double(i)/100.0 ;
		double x = 0.03*cos(ang);
		double y = 0.03*sin(ang);
		glVertex2f(p.pos_x + del_x + x , p.pos_y + del_y + y);
	}
	glEnd() ;
	glColor4f(0.0,0.0,0.0,1.0);
	glBegin(GL_POLYGON);
	for(int i = 0 ; i < 100 ; i++)
	{
		double ang = 2.0* 3.1415926 * double(i)/100.0 ;
		double x = 0.01*cos(ang);
		double y = 0.01*sin(ang);
		glVertex2f(p.pos_x + x , p.pos_y + y);
	}
	glEnd() ;
	double xtheta = (mx-p.pos_x)/(sqrt(pow(mx-p.pos_x,2)+pow(my-p.pos_y,2))) ;
	double ytheta = (my-p.pos_y)/(sqrt(pow(mx-p.pos_x,2)+pow(my-p.pos_y,2))) ;

	glBegin(GL_LINES);
	glColor4f(0.6,0.0,0.0,1.0);
	glVertex2f(p.pos_x - 0.031*ytheta,p.pos_y + 0.031*xtheta);
	glVertex2f(p.pos_x - 0.031*ytheta + 0.028*xtheta,p.pos_y + 0.031*ytheta + 0.028*xtheta);
	glEnd();

	glBegin(GL_LINES);
	glColor4f(0.6,0.0,0.0,1.0);
	glVertex2f(p.pos_x + 0.031*ytheta,p.pos_y - 0.031*xtheta);
	glVertex2f(p.pos_x + 0.031*ytheta + 0.028*xtheta,p.pos_y - 0.031*xtheta + 0.028*ytheta);
	glEnd();

}

void Display()																		//Function called by glutDisplayFunc()
{

	glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < wall_coordinates.size(); ++i)
    {
    	Draw_wall(wall_coordinates[i]);
    }
    Draw_man(players[0]);
	//glColor3f(0.45f,0.7f,0.45f);
	glLoadIdentity();
	glFlush() ;
}

void mouse(int x,int y)																	//invoked whenever the mouse pointer moves
{
	static double mini = 1.0;

	if(winwidth>winheight)
	{
		mini = winheight;
		aspectx = (double)winwidth / (double)winheight;
		aspecty = 1.0;
	}
	else
	{
		mini = winwidth;
		aspectx = 1.0;
		aspecty = (double)winheight / (double)winwidth;
	}
	mx = ((double)x / (double)mini - 0.5*aspectx) * 2.0;
	my = ((double)-y / (double)mini + 0.5*aspecty) * 2.0;
}

void Keys(int key , int a , int b)																		//Invoked everytime a key is pressed
 {
	static int v = 0;	
	double xtheta = (mx-players[0].pos_x)/(sqrt(pow(mx-players[0].pos_x,2)+pow(my-players[0].pos_y,2))) ;
	double ytheta = (my-players[0].pos_y)/(sqrt(pow(mx-players[0].pos_x,2)+pow(my-players[0].pos_y,2))) ;																				//Tells which ball is selected

	if(players.size()!=0)
	{
		if (key == GLUT_KEY_RIGHT){
			players[0].pos_x += .01*ytheta;
			players[0].pos_y -= .01*xtheta;
		}
		else if (key == GLUT_KEY_LEFT){
			players[0].pos_x -= .01*ytheta;
			players[0].pos_y += .01*xtheta;
		}
		else if (key == GLUT_KEY_UP){
			players[0].pos_x += .01*xtheta;
			players[0].pos_y += .01*ytheta;		
		}
		else if (key == GLUT_KEY_DOWN){
			players[0].pos_x -= .01*xtheta;
			players[0].pos_y -= .01*ytheta;		
		}

		#ifdef DEBUG
		cout<<"Keyboard event"<<std::endl;
		#endif
		glutPostRedisplay();
	}
}


void update(int value)											//invoked repeatedly after particular time interval. Threads are created and destroyed here
{
	glutTimerFunc(4,update,value);
	glutPostRedisplay();
}


void Window_resize(int width ,int height)												//adjusts the display on window resizing
{
	winwidth = width;
	winheight = height;
	glViewport(0, 0, width, height);													 // reset the viewport
	glMatrixMode(GL_PROJECTION);														 // modify the projection matrix
	glLoadIdentity();          															 // load an identity matrix into the projection matrix
	if(width>height)
	{
		tup = 1.0 ; tdown = -1.0 ;
		tleft = -((float)width/(float)height) ; tright = ((float)width/(float)height) ;
	}
	else
	{
		tleft = -1.0 ; tright = 1.0 ;
		tup = ((float)height/(float)width) ; tdown = -((float)height/(float)width) ;
	}
	if(winwidth>winheight)
	{
	aspectx = (double)winwidth / (double)winheight;
	aspecty = 1.0;
	}
	else
	{
	aspectx = 1.0;
	aspecty = (double)winheight / (double)winwidth;
	}

//	#ifdef DEBUG
//	cout<<"Window resized"<<endl;
//	#endif

	glOrtho(tleft, tright, tdown,tup , -1.0, 1.0); 										// create new projection matrix
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}


void give_map_coordinates(){
	wall_coordinates.push_back(pair<pair<float,float>,pair<float,float> > (pair<float,float>(-0.6,0),pair<float,float>(-0.2,0.01)));
	wall_coordinates.push_back(pair<pair<float,float>,pair<float,float> > (pair<float,float>(0.1,0),pair<float,float>(0.6,0.01)));
	wall_coordinates.push_back(pair<pair<float,float>,pair<float,float> > (pair<float,float>(0.6,0.01),pair<float,float>(0.59,0.6)));
	wall_coordinates.push_back(pair<pair<float,float>,pair<float,float> > (pair<float,float>(0.4,0.81),pair<float,float>(0.8,0.8)));
	wall_coordinates.push_back(pair<pair<float,float>,pair<float,float> > (pair<float,float>(0,0),pair<float,float>(-0.6,0.01)));
	wall_coordinates.push_back(pair<pair<float,float>,pair<float,float> > (pair<float,float>(-0.6,0.01),pair<float,float>(-0.59,-0.6)));
	wall_coordinates.push_back(pair<pair<float,float>,pair<float,float> > (pair<float,float>(-0.4,-0.81),pair<float,float>(-0.8,-0.8)));
	wall_coordinates.push_back(pair<pair<float,float>,pair<float,float> > (pair<float,float>(-0.3,-0.2),pair<float,float>(0.3,-0.19)));
	wall_coordinates.push_back(pair<pair<float,float>,pair<float,float> > (pair<float,float>(-0.3,0.2),pair<float,float>(0.3,0.19)));
	wall_coordinates.push_back(pair<pair<float,float>,pair<float,float> > (pair<float,float>(0.3,-0.19),pair<float,float>(0.31,-0.59)));
	wall_coordinates.push_back(pair<pair<float,float>,pair<float,float> > (pair<float,float>(-0.3,0.19),pair<float,float>(-0.31,0.6)));
	wall_coordinates.push_back(pair<pair<float,float>,pair<float,float> > (pair<float,float>(0.65,-1.0),pair<float,float>(0.66,-0.45)));
	wall_coordinates.push_back(pair<pair<float,float>,pair<float,float> > (pair<float,float>(-0.65,1.0),pair<float,float>(-0.66,0.45)));
	wall_coordinates.push_back(pair<pair<float,float>,pair<float,float> > (pair<float,float>(1.0,-0.25),pair<float,float>(0.66,-0.26)));
	wall_coordinates.push_back(pair<pair<float,float>,pair<float,float> > (pair<float,float>(-1.0,0.25),pair<float,float>(-0.66,0.26)));
	wall_coordinates.push_back(pair<pair<float,float>,pair<float,float> > (pair<float,float>(0.1,-0.4),pair<float,float>(-0.35,-0.41)));
	wall_coordinates.push_back(pair<pair<float,float>,pair<float,float> > (pair<float,float>(0.1,-0.4),pair<float,float>(0.11,-0.7)));
	wall_coordinates.push_back(pair<pair<float,float>,pair<float,float> > (pair<float,float>(-0.35,-0.40),pair<float,float>(-0.34,-0.6)));
	wall_coordinates.push_back(pair<pair<float,float>,pair<float,float> > (pair<float,float>(-0.34,-0.59),pair<float,float>(-0.1,-0.6)));
	wall_coordinates.push_back(pair<pair<float,float>,pair<float,float> > (pair<float,float>(0.2,0.6),pair<float,float>(0.21,1.0)));
	wall_coordinates.push_back(pair<pair<float,float>,pair<float,float> > (pair<float,float>(0,0.8),pair<float,float>(0.01,1.0)));
	wall_coordinates.push_back(pair<pair<float,float>,pair<float,float> > (pair<float,float>(0,0.6),pair<float,float>(0.01,1.0)));
	wall_coordinates.push_back(pair<pair<float,float>,pair<float,float> > (pair<float,float>(0,0.5),pair<float,float>(0.21,0.51)));
}

void give_player_coordinates()
{
	player a;
	a.pos_x=-.9;
	a.pos_y=-.9;
	players.push_back(a);
}

int main(int argc ,char** argv)
{
	int handle = socket( AF_INET, 
                     SOCK_DGRAM, 
                     IPPROTO_UDP );

	if ( handle <= 0 )
	{
	    printf( "failed to create socket\n" );
	    return false;
	}
	int portno = atoi(argv[1]);
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = 
	htons( (unsigned short) portno );

	if ( bind( handle, 
	           (const sockaddr*) &address, 
	           sizeof(sockaddr_in) ) < 0 )
	{
	    printf( "failed to bind socket\n" );
	    return false;
	}
	int num;
	char packet_data[256];
	sockaddr_in dest;
	dest.sin_family = AF_INET;

    unsigned int max_packet_size = 
    sizeof( packet_data );

    sockaddr_in from;
    socklen_t fromLength = sizeof( from );
	unsigned int from_address;
    unsigned short from_port;
	int sent_bytes; 
    char ipdata[256];
    char player_number[256];
    stringstream s;
    int bytes;
    char option;
    cout<<"1.Host game\n2.Join game\nSelect : ";
    cin>>option;
    if(option=='1')
    {
	cout<<"Enter no.of players who will join : ";
	cin>>num;
    for(int i=0;i<num;i++)
    {

    bytes = recvfrom( handle, 
                          (char*)packet_data, 
                          max_packet_size,
                          0, 
                          (sockaddr*)&from, 
                          &fromLength );
    
    from_address = ntohl( from.sin_addr.s_addr );
    from_port = ntohs( from.sin_port );
        
    player_ip.push_back(pair<unsigned int,unsigned short>(from_address,from_port));

	//cout<<from_port<<endl;
	}
	s<<player_ip.size();
	strcpy(player_number,const_cast<char *>((s.str()).c_str()));
	for(int i = 0;i<player_ip.size();i++)
	{
        		dest.sin_addr.s_addr = htonl(player_ip[i].first);
				dest.sin_port = htons(player_ip[i].second);
				cout<<"yo "<<i<<"   "<<player_number;
				sent_bytes = sendto( handle, 
            	player_number, 
            	256,
            	0, 
            	(sockaddr*)&dest, 
            	sizeof(sockaddr_in) );

            	bytes = recvfrom( handle, 
                          (char*)packet_data, 
                          max_packet_size,
                          0, 
                          (sockaddr*)&from, 
                          &fromLength );

		for(int j=0;j<player_ip.size();j++)
        {
        	if(j!=i)
        	{
        		stringstream s1,s2;
        		from_address = player_ip[j].first;
        		from_port = player_ip[j].second;
				s1<<from_address;
				strcpy(ipdata,const_cast<char *>((s1.str()).c_str()));
				sent_bytes = sendto( handle, 
            	ipdata, 
            	256,
            	0, 
            	(sockaddr*)&dest, 
            	sizeof(sockaddr_in) );
            	cout<<ipdata<<"  ";


				bytes = recvfrom( handle, 
                          (char*)packet_data, 
                          max_packet_size,
                          0, 
                          (sockaddr*)&from, 
                          &fromLength );

				s2<<from_port;
				strcpy(ipdata,const_cast<char *>((s2.str()).c_str()));
				s.clear();
				sent_bytes = sendto( handle, 
            	ipdata, 
            	256,
            	0, 
            	(sockaddr*)&dest, 
            	sizeof(sockaddr_in) );
            	cout<<ipdata<<"  ";
			}

        }
    }
	}
	else if(option == '2')
	{
		cout<<"Enter ip address : ";
		string ip_add;
		cin>>ip_add;
		unsigned int dots = 0,a,b,c,d,at = 0;
		for (int i = 0; i < ip_add.size(); ++i)
		{
			if(ip_add[i] == '.' && dots == 0){
				a = atoi(ip_add.substr(0,i).c_str()) ;
				at = i ;
				dots++ ;
			}
			else if(ip_add[i] == '.' && dots == 1){
				b = atoi(ip_add.substr(at+1,i-at-1).c_str()) ;
				at = i ;
				dots++ ;
			}
			else if(ip_add[i] == '.' && dots == 2){
				c = atoi(ip_add.substr(at+1,i-at-1).c_str()) ;
				at = i ;
				dots++ ;
			}
			else if(dots == 3){
				d = atoi(ip_add.substr(at+1,ip_add.size()-1-at).c_str()) ;
			}
		}
		cout<<a<<"  "<<b<<"   "<<c << "   "<< d<< "" <<endl;
		cout<<"\nEnter port number : ";
		unsigned short po;
		cin>>po;
		unsigned int hostaddr = ( a << 24 ) | 
                       ( b << 16 ) | 
                       ( c << 8  ) | 
                         d;
        
    	player_ip.push_back(pair<unsigned int,unsigned short>(hostaddr,po));
	
		sockaddr_in host;
		host.sin_family = AF_INET;
		host.sin_addr.s_addr = htonl( hostaddr);
		host.sin_port = htons( po );
		char *ab = new char[256];
		ab = "connected";
		sent_bytes = 
    		sendto( handle, 
            (const char*)ab, 
            256,
            0, 
            (sockaddr*)&host, 
            sizeof(sockaddr_in) );
        bytes = recvfrom( handle, 
                          (char*)packet_data, 
                          max_packet_size,
                          0, 
                          (sockaddr*)&from, 
                          &fromLength );
        sent_bytes = 
    		sendto( handle, 
            (const char*)ab, 
            256,
            0, 
            (sockaddr*)&host, 
            sizeof(sockaddr_in) );
        int vsize = atoi(packet_data)-1;
        char vaddress[256];
        char vport[256];
        unsigned int vaddr;
        unsigned short vportno;
        for(int i=0;i<vsize;i++)
        {
        	bytes = recvfrom( handle, 
                          (char*)vaddress, 
                          max_packet_size,
                          0, 
                          (sockaddr*)&from, 
                          &fromLength );

        	sent_bytes = 
    		sendto( handle, 
            (const char*)ab, 
            256,
            0, 
            (sockaddr*)&host, 
            sizeof(sockaddr_in) );
            
        	bytes = recvfrom( handle, 
                          (char*)vport, 
                          max_packet_size,
                          0, 
                          (sockaddr*)&from, 
                          &fromLength );
        	vaddr = atoi(vaddress);
        	vportno = atoi(vport);
        	cout<<vaddr<<"  "<<vportno;
        	player_ip.push_back(pair<unsigned int,unsigned short>(vaddr,vportno));
        }

	}
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(winwidth,winheight);
	glutCreateWindow ("The Last of Us");
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	give_map_coordinates();
	give_player_coordinates();
	glutDisplayFunc(Display);
	glutSpecialFunc(Keys);																			//For keyboard functionality
//	glutMouseFunc(click);																			//Mouse click functionality
	glutPassiveMotionFunc(mouse);																	//Mouse motio functionality
//	glutReshapeFunc(Window_resize) ;
	glutTimerFunc(4,update,0);
//	menu();
	glutMainLoop();

	return 0 ;
}