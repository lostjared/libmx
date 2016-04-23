/*

	object oriented 3D example of MX

			jared@lostsidedead.biz


*/


#include<mx.h>
#include<mx3d_math.h>


static const int uni_rad = 4000, point_size = 200, num_points_x = 2*uni_rad/point_size, num_points_y = 2*uni_rad/point_size;
static const int num_points_z = num_points_y*num_points_z;
static const int num_towers = 96, num_tanks = 24, tank_speed = 25;


class mainWindow : public mx::mxWnd {

public:

	mainWindow() : mxWnd(720, 480, 0),cam_pos(0,40,40,1), cam_target(0,0,0,1), cam_dir(0,0,0,1), vscale(1.0f,1.0f,1.0f,1.0f),vrot(0,0,0,1),vpos(0,0,0,1), turning(0)
	{

		setTitle("mx3d Demo");

		axis[0] = 0;
		axis[1] = 0;

		mx3d::BuildTables();

		cam.Init(0,cam_pos,cam_dir,0,200.0f,12000.0f,120.0f,float(front.getSurface()->w),float(front.getSurface()->h));
        	vscale.Set(0.75f,0.75f,0.75f,1.0f);
	        if(!tank.LoadPLG("mesh/tank2.plg", vscale, vpos, vrot, SDL_MapRGB(front.getSurface()->format, 200,200,200))) 
                	throw mx::mxException<std::string>( " could not load model.. \n");

	        	vscale.Set(0.75,0.75,0.75,1.0f);

        	vscale.Set(1.0f,2.0f,1.0f,1.0f);
        	if(!tower.LoadPLG("mesh/tower.plg", vscale, vpos, vrot, SDL_MapRGB(front.getSurface()->format, 255,200,255)))
               		throw mx::mxException<std::string>( " could not load model.. \n");

        	vscale.Set(3.0f,3.0f,3.0f,1.0f);
        	if(!marker.LoadPLG("mesh/marker.plg", vscale, vpos, vrot, SDL_MapRGB(front.getSurface()->format, 100,100,100)))
               		throw mx::mxException<std::string>( " could not load model.. \n");


        	int index;

        	for(index = 0; index < num_tanks; index++) {
                	tank_points[index].x = float(mx3d::rrand(-uni_rad, uni_rad));
                	tank_points[index].y = 0;
                	tank_points[index].z = float(mx3d::rrand(-uni_rad, uni_rad));
                	tank_points[index].w = float(mx3d::rrand(0,360));
        	}

        	for(index = 0; index < num_towers; index++) {
                	tower_points[index].x = float(mx3d::rrand(-uni_rad, uni_rad));
                	tower_points[index].y = 0;
                	tower_points[index].z = float(mx3d::rrand(-uni_rad, uni_rad));
        	}

	}


	virtual void eventPassed(SDL_Event &e)
	{

		switch(e.type)
		{
		case SDL_QUIT:
		quit();
		break;


		case SDL_KEYDOWN:
		if(e.key.keysym.sym == SDLK_ESCAPE) quit();
		break;

		}
	}

	virtual void renderScreen()
	{
		static float tank_speed = 0.0f;

		SDL_FillRect(front, 0, SDL_MapRGB(front.getSurface()->format, 0, 0, 0));

	        mx3d::vec4D pos(0,0,0,0);

        	rl.Reset();
        	tank_speed = 25;



        	cam.BuildEuler(5);
        	int i;



        	for(i = 0; i < num_tanks; i++) {
                	tank.Reset();
                	mrot.BuildXYZ(0,tank_points[i].w, 0);
                	tank.TransformObject(mrot,1);
                	tank.world_pos.x = tank_points[i].x;
                	tank.world_pos.y = tank_points[i].y;
                	tank.world_pos.z = tank_points[i].z;
                	if(!CullObject(tank,cam)) {
                        	tank.ModelToWorld();
                        	tank.BuildRenderList(rl);
                	}
        	}

        	mrot.BuildXYZ(0,cam.dir.y+turning,0);
        
        	for(i = 0;  i < num_towers; i++) {
                	tower.Reset();
                	tower.world_pos.x = tower_points[i].x;
                	tower.world_pos.y = tower_points[i].y;
                	tower.world_pos.z = tower_points[i].z;
                	if(!CullObject(tower,cam)) {
                        	tower.ModelToWorld();
                        	tower.BuildRenderList(rl);
                	}
        	}

        	rl.RemoveFaces(cam.pos);
        	cam.WorldToCamera(rl);
        	cam.CameraToPerspective(rl);
        	cam.PerspectiveToScreen(rl);
        	graph.Begin(mx);
        	graph.DrawSolidPolys(rl);
        	graph.End();

		front.Flip();

		keyScan();

	}

protected:

	mx3d::Camera cam;
	mx3d::mxObject tower,tank,marker;
	mx3d::vec4D tower_points[num_towers], tank_points[num_tanks];
	mx3d::RenderList rl;
	mx3d::PipeLine graph;
	mx3d::Mat4D mrot;
	mx3d::vec4D cam_pos, cam_target, cam_dir, vscale,vrot,vpos;
	float turning;
	int axis[2];
	mx3d::graphicsInterface mx;

	bool CullObject(mx3d::mxObject &o, mx3d::Camera &cam) 
	{
        
		return (o.CullObject(true,true,true,cam.mcam,cam.far_clip_z,cam.near_clip_z,cam.viewplane_width, cam.viewplane_height, cam.view_dist));
	}

	void keyScan()
	{

	        Uint8 *keybuf = SDL_GetKeyState(0);

	        if(keybuf[SDLK_UP])
        	{
        	        cam.pos.x += tank_speed*mx3d::fast_sinf(cam.dir.y);
                	cam.pos.z += tank_speed*mx3d::fast_cosf(cam.dir.y);
        	}
        	else if (keybuf[SDLK_DOWN])
        	{
                	cam.pos.x -= tank_speed*mx3d::fast_sinf(cam.dir.y);
                	cam.pos.z -= tank_speed*mx3d::fast_cosf(cam.dir.y);
        	}
        	else if (keybuf[SDLK_RIGHT])
        	{
                	cam.dir.y += 3;
                	if((turning += 2) > 15)
                        	turning = 15;
	        }
        	else if(keybuf[SDLK_LEFT])
        	{
                	cam.dir.y -= 3;
                	if((turning -= 2) < -15)
                        	turning = -15;
	        }
        	else
        	{
                	if(turning > 0)
                        	turning -= 1;
                	else if(turning < 0)
                        	turning += 1;
        	}
	}

};





int main(int argc, char **argv)
{


	try 
	{

		mainWindow w;
		return w.messageLoop();

	}
	catch ( mx::mxException<std::string> &e )
	{

		e.printError(std::cerr);
	}
	catch ( std::exception &e )
	{
		std::cerr << e.what() << "\n";
	}
	catch ( ... )
	{
		std::cerr << "undefined exception\n";
	}

	return EXIT_FAILURE;
}

