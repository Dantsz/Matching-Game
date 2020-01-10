#include <Empaerior.h>
#include <main.h>
#include "Cards.h"
//An example of what a application might look like

//a user defined state
class APP_State : public Empaerior::State
{

public:
	APP_State()
	{
		
        ecs.Init();
		ecs.register_component<Empaerior::Position_Component>();
		ecs.register_component<Empaerior::Camera_Component>();
		ecs.register_component<Empaerior::Sprite_Component>();
		ecs.register_component<Empaerior::Event_Listener_Component>();
		ecs.register_component<Card_component>();

		spr_system = ecs.register_system <Sprite_System>();
		event_system = ecs.register_system<Event_System>();
		spr_cards = ecs.register_system<Card_System>();


		ecs.add_component_to_system<Empaerior::Sprite_Component, Sprite_System>();
		ecs.add_component_to_system<Empaerior::Event_Listener_Component, Event_System>();
		ecs.add_component_to_system<Card_component, Card_System>();

		spr_cards->load_assets();
		spr_cards->load_board(ecs);


	}

	void Update(const Empaerior::u_s_int& dt)override
	{
		unsigned char const* keys = SDL_GetKeyboardState(nullptr);
		if (keys[SDL_SCANCODE_UP])
		{
			Empaerior::Application::cur_state->get_camera().set_dimensions(Empaerior::Application::cur_state->get_camera().rect.w + 12, Empaerior::Application::cur_state->get_camera().rect.h + 10);
		}
		else if (keys[SDL_SCANCODE_DOWN])
		{
			Empaerior::Application::cur_state->get_camera().set_dimensions(Empaerior::Application::cur_state->get_camera().rect.w - 12, Empaerior::Application::cur_state->get_camera().rect.h - 10);
		}
		else if (keys[SDL_SCANCODE_W])
		{
			Empaerior::Application::cur_state->get_camera().set_position(Empaerior::Application::cur_state->get_camera().rect.x, Empaerior::Application::cur_state->get_camera().rect.y - 10);
		}
		else if (keys[SDL_SCANCODE_S])
		{
			Empaerior::Application::cur_state->get_camera().set_position(Empaerior::Application::cur_state->get_camera().rect.x, Empaerior::Application::cur_state->get_camera().rect.y + 10);
		}
		else if (keys[SDL_SCANCODE_A])
		{
			Empaerior::Application::cur_state->get_camera().set_position(Empaerior::Application::cur_state->get_camera().rect.x - 10, Empaerior::Application::cur_state->get_camera().rect.y);
		}
		else if (keys[SDL_SCANCODE_D])
		{
			Empaerior::Application::cur_state->get_camera().set_position(Empaerior::Application::cur_state->get_camera().rect.x + 10, Empaerior::Application::cur_state->get_camera().rect.y);
		}


		spr_system->update(ecs, dt);
	}
	
	virtual void Render() override//renders the state
	{
		
		spr_system->render(ecs,camera);
	
	}
	virtual void handleevents(const SDL_Event& event) override
	{
		event_system->handle_events(ecs, event);
	}

private:
	std::shared_ptr<Card_System> spr_cards;
	std::shared_ptr<Sprite_System> spr_system;
	std::shared_ptr<Event_System> event_system;

};

//a user defined application
class Test_Aplication : public Empaerior::Application
{
public:
	Test_Aplication()
	{
		window.Init("test", 960, 800);

		states.emplace_back(new APP_State());//add a new state

		set_state(states[0]);

		SDL_RenderSetLogicalSize(Application::window.renderer, Application::cur_state->get_camera().rect.w, Application::cur_state->get_camera().rect.h);

	}

	~Test_Aplication()
	{

	}

	//the main loop

	void run() override
	{
		Empaerior::u_s_int framestart = 0;
		Empaerior::u_s_int frametime = 0;
		Empaerior::u_s_int currentime = 0;
		Empaerior::u_s_int acumulator = 0;

		while (Empaerior::Application::is_running)
		{





	
			while (SDL_PollEvent(&Empaerior::Application::event)) {

				handlevents(Empaerior::Application::event);

			}
			if (!Empaerior::Application::is_paused)
			{


				framestart = SDL_GetTicks();
				frametime = framestart - currentime;

				if (frametime > 25) frametime = 25; //if too many frames are skipped

				currentime = framestart;
				acumulator += frametime;



				while (acumulator >= Empaerior::Application::dt)
				{


					//update 

					Update(Empaerior::Application::dt);

					acumulator -= Empaerior::Application::dt;



				}


				//Text_Sprite * norge = new Text_Sprite({ 0,0,200,200 }, "assets/font.ttf", 32 ,s, color);




				Empaerior::Application::window.clear();
				render();

				Empaerior::Application::window.render();


			}

			Empaerior::Asset_Loading::clean_textures();

		}
	}


	void handlevents(const SDL_Event& event) override
	{
		Empaerior::Application::window.window_listener.handleEvents(event);
		cur_state->handleevents(event);
	}
	void Update(const unsigned int& dt)override
	{
		cur_state->Update(Application::dt);
	}


	void render() override
	{
		cur_state->Render();
	}

	



};


Empaerior::Application* Empaerior::Create_Application()
{
	return new Test_Aplication();
}