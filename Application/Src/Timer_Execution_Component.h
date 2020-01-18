#pragma once
//Executes a function after a certain amount of time has passed
#include <Empaerior.h>


struct T_E_Component
{
	
	using Timed_Function = std::function<void()>;
	std::vector<Empaerior::u_inter> elapsed_time;
	std::vector<Empaerior::u_inter> time;
	std::vector<Timed_Function> functions;
};

class T_E_System : public Empaerior::System
{
public:
	using Timed_Function = std::function<void()>;
	void add_function(Empaerior::ECS& ecs, const Empaerior::u_inter& id,const Empaerior::u_inter& time, Timed_Function function)
	{
		ecs.get_component<T_E_Component>(id).time.emplace_back(time);
		ecs.get_component<T_E_Component>(id).functions.emplace_back(function);
		ecs.get_component<T_E_Component>(id).elapsed_time.emplace_back(0);

	}

#define component ecs.get_component<T_E_Component>(id)
	void Update(Empaerior::ECS& ecs, const Empaerior::u_int& dt)
	{
		for (auto& id : entities_id)
		{
			for (Empaerior::u_inter i = 0; i < component.functions.size(); i++)
			{
				component.elapsed_time[i] += dt;
				//if enought time has passed
				if (component.elapsed_time[i] >= component.time[i])
				{
					//call function
					component.functions[i]();
					//erase
					component.elapsed_time.erase(component.elapsed_time.begin() + i);
					component.time.erase(component.time.begin() + i);
					component.functions.erase(component.functions.begin() + i);

				}

			}

		}

	}
#undef component


};