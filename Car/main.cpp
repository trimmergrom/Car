#include<iostream>
#include<conio.h>
#include<thread>
#include<Windows.h>

using namespace std::chrono_literals;


using std::cout;
using std::cin;
using std::endl;

#define MIN_TANK_VOLUME 40
#define MAX_TANK_VOLUME 80
#define DEFOULT_TANK_VOLUME 60

class Tank
{
	const int VOLUME;
	double fuel_level;
public:
	int get_volume()const
	{
		return VOLUME;
	}
	double get_fuel_level()const
	{
		return fuel_level;
	}
	double give_fuel(double amount)
	{
		if (fuel_level - amount > 0)fuel_level -= amount;
		else fuel_level = 0;
		return fuel_level;
	}
	void fill(double amount)
	{
		if (amount < 0)return;
		if (fuel_level + amount < VOLUME)fuel_level += amount;
		else fuel_level = VOLUME;
	}
	void info()const
	{
		system("CLS");
		cout << "Volum:      " << VOLUME << "liters\n";
		cout << "Fuel level: " << fuel_level << "liters\n";
	}
	Tank(int volume):VOLUME(volume>=MIN_TANK_VOLUME&&volume<=MAX_TANK_VOLUME?volume:DEFOULT_TANK_VOLUME)
	{		
		this->fuel_level = 0;
		cout << "Tank is ready: " << this << endl;
	}
	~Tank()
	{
		cout << "Tank is over: " << this << endl;
	}
};
#define MIN_ENGINE_CONSUMPTION 5
#define MAX_ENGINE_CONSUMPTION 40
#define DEFOULT_ENGINE_CONSUMPTION 25
#define DEFOULT_IDLE_INSTANT_CONSUMPTION 0,8
#define IDLE_RPM 800
#define MAX_RPM 6000
class Engine
{
	double consumption;
	double instant_consumption;
	double default_instant_consumption;
	double rpm;
	bool is_started;
public:
	double get_consumption()const
	{
		return consumption;
	}
	double get_instant_consumption()const
	{
		return instant_consumption;
	}
	void set_instant_consumption(double instant_consumption)
	{
		//this->consumption = consumption >= MIN_ENGINE_CONSUMPTION && consumption <= MAX_ENGINE_CONSUMPTION ? consumption : DEFOULT_ENGINE_CONSUMPTION;
		//this->default_instant_consumption = this->instant_consumption = this->consumption * 3e-4;		
		this->default_instant_consumption = this->instant_consumption = this->consumption;		
	}
	/*void set_instant_consumption(double rpm)
	{
		if (rpm == 0)instant_consumption = 0;
		else if (rpm == IDLE_RPM)instant_consumption = DEFOULT_IDLE_INSTANT_CONSUMPTION;
		else if (rpm > IDLE_RPM && rpm < MAX_RPM) instant_consumption = DEFOULT_IDLE_INSTANT_CONSUMPTION + DEFOULT_IDLE_INSTANT_CONSUMPTION * pow(rpm / 1000, 2);
		
	}*/
	void set_rpm(double instant_consumption)
	{
		if (instant_consumption == 0) rpm = 0;
		else if (DEFOULT_IDLE_INSTANT_CONSUMPTION) rpm = IDLE_RPM;
		else if (instant_consumption > DEFOULT_IDLE_INSTANT_CONSUMPTION && instant_consumption < MAX_ENGINE_CONSUMPTION)
			rpm = IDLE_RPM + pow(instant_consumption / 0.4, 0.5);
	}

	void start()
	{
		is_started = true;
	}
	void stop()
	{
		is_started = false;
	}
	bool control_started()const
	{
		return is_started;
	}
	void info()const
	{
		cout << "Consumption per 100 km: " << consumption << " liters\n";
		cout << "RPM: " << rpm << " rpm\n";
		cout << "Instant Consumption per hour: " << instant_consumption << " liters/hour.\n";
		cout << "Engine is: " << (is_started ? "started" : "stopped") << endl;
	}
	Engine(double instant_consumption, double rpm)
	{
		set_instant_consumption(instant_consumption);
		this->rpm = 0;
		this->is_started = false;
		cout << "Engine is ready: " << this << endl;
	}
	~Engine()
	{
		cout << "Engine is over: " << this << endl;
	}
	/*void speed_consumption_dependency(int speed)
	{
		if (speed == 0)instant_consumption = default_instant_consumption;
		else if (speed < 60)instant_consumption = default_instant_consumption*10;
		else instant_consumption = default_instant_consumption*12;
	}*/
	void rpm_consumption_dependency(double instant_consumption)
	{
		if (instant_consumption == 0) rpm = 0;
		else if (instant_consumption = DEFOULT_IDLE_INSTANT_CONSUMPTION) rpm == IDLE_RPM;
		else if (instant_consumption > DEFOULT_IDLE_INSTANT_CONSUMPTION && instant_consumption < MAX_ENGINE_CONSUMPTION)
			rpm = IDLE_RPM + pow(instant_consumption / 0.4, 0.5);		
	}
	void free_rotation()
	{
		while (rpm > IDLE_RPM)
		{
			rpm--;
			if (rpm < IDLE_RPM)rpm = IDLE_RPM;
			//rpm_consumption_dependency(rpm);
			std::this_thread::sleep_for(500ms);
		}
	}


};

#define MAIN_GEAR 4.5
#define D_1 3.8
#define D_2 2.75
#define D_3 1.55
#define D_4 1
#define D_5 0.85
class  Transmission
{
	bool clath;
	bool d_1;
	bool d_2;
	bool d_3;
	bool d_4;
	bool d_5;
public:
	void drive()
	{
		clath = true;
	}
	void stop_drive()
	{
		clath = false;
	}
	bool control_clath()
	{
		return clath;
	}
	 Transmission();
	~ Transmission();



};

 Transmission:: Transmission()
{
}

 Transmission::~ Transmission()
{
}

#define MAX_SPEED_LOW_LIMIT 60
#define MAX_SPEED_HIGTH_LIMIT 300
#define DEFOULT_SPEED 200
#define Escape 27
#define Enter 13
class Car
{
	Engine engine;
	Tank tank;
	bool driver_inside;
	double speed;
	double rpm;
	const int MAX_SPEED;
	int acceleration;
	struct Control
	{
		std::thread panel_thread;
		std::thread engine_idle_thread;
		std::thread free_wheeling_thread;
		std::thread rpm_thread;
	}control;
public:
	Car(double consumption, int volume, double rpm, int max_speed ) :
		engine(consumption, rpm), tank(volume),
		MAX_SPEED(max_speed >= MAX_SPEED_LOW_LIMIT && max_speed <= MAX_SPEED_HIGTH_LIMIT ? max_speed : DEFOULT_SPEED)
	{
		driver_inside = false;
		speed = 0;
		double rpm = 0;
		acceleration = MAX_SPEED / 10;
		cout << "Your car is ready to go: " << this << endl;
	}
	~Car()
	{
		cout << "Your caar is over: " << this << endl;
	}
	void fill(double amount)
	{
		tank.fill(amount);
	}
	void get_in()
	{
		driver_inside = true;
		//panel();
		control.panel_thread = std::thread(&Car::panel, this);
	}
	void get_out()
	{
		driver_inside = false;
		system("CLS");
		if (control.panel_thread.joinable())control.panel_thread.join();
		cout << "You are out of your caar" << endl;
	}
	void start_engine()
	{
		if (driver_inside && tank.get_fuel_level())
		{
			engine.start();
			control.engine_idle_thread = std::thread(&Car::idle_engine, this);
		}
	}
	void stop_engine()
	{
		engine.stop();
		if (control.engine_idle_thread.joinable())
		{
			control.engine_idle_thread.join();
		}
	}
	void idle_engine()
	{
		while (engine.control_started()&&tank.give_fuel(engine.get_instant_consumption()))
		{
			std::this_thread::sleep_for(1s);
			if (!tank.get_fuel_level())engine.stop();
		}
	}
	void free_wheeling()
	{
		while (speed > 0)
		{
			speed--;
			if (speed < 0)speed = 0;
			engine.rpm_consumption_dependency(speed);
			std::this_thread::sleep_for(1s);
		}
	}
	
	void control_car()
	{
		char key;
		do
		{
			key = _getch();
			switch(key)
			{
			case Enter: driver_inside ? get_out() : get_in(); break;
			case 'F': case'f':
				double fuel;
				cout << "Enter fuel value: "; cin >> fuel;
				fill(fuel);
				break;
			case 'I': case 'i'://Ignition
				engine.control_started() ? stop_engine() : start_engine();
				break;
			/*case 'W':case'w':
				if (driver_inside && engine.control_started())
				{
					if (speed < MAX_SPEED)speed += acceleration;
					if (speed > MAX_SPEED)speed = MAX_SPEED;
					std::this_thread::sleep_for(950ms);
					if (!control.free_wheeling_thread.joinable())
						control.free_wheeling_thread = std::thread(&Car::free_wheeling, this);
				}*/
			case 'W':case'w':
				if (driver_inside && engine.control_started())
				{
					if (rpm < MAX_RPM)engine.instant_consuption += acceleration;
					if (rpm > MAX_RPM)rpm = MAX_RPM;
					std::this_thread::sleep_for(950ms);
					if (!control.free_wheeling_thread.joinable())
						control.free_wheeling_thread = std::thread(&Car::free_wheeling, this);
				}
				break;
			case 'S':case 's':
				if (driver_inside && speed > 0)
				{
					speed -= acceleration;
					if (speed < 0)speed = 0;
					std::this_thread::sleep_for(950ms);
				}
				break;
			case Escape:
				speed = 0;
				get_out();
				stop_engine();
				break;				
			}
			if (!engine.control_started() && control.engine_idle_thread.joinable())
				control.engine_idle_thread.join();
			if (speed == 0 && control.free_wheeling_thread.joinable())
				control.free_wheeling_thread.join();
			
			

		} while (key != Escape);
	}
	void panel()const
	{
		
		while (driver_inside)
		{
			system("CLS");
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(hConsole, 0xCF);
			for (int i = 0; i < speed / 8; i++)
			{
				cout << "|";
			}
				SetConsoleTextAttribute(hConsole, 0x07);
			cout << endl;
			cout << "Speed: " << speed << " km/h\n";
			cout << "Fuel level: " << tank.get_fuel_level() << " liters";
			if (tank.get_fuel_level() < 5)
			{
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(hConsole, 0xCF);
				cout << " LOW FUEL";
				SetConsoleTextAttribute(hConsole, 0x07);

			}
			cout << endl;
			cout << "Engine is " << (engine.control_started() ? "started" : "stopped") << endl;
			cout << "Instante consumption: " << engine.get_instant_consumption() << " liters/sec\n";
			std::this_thread::sleep_for(200ms);
		}
	}



};


//#define TANK_CHECK
//#define ENGINE_CHECK

void main()
{
#ifdef TANK_CHECK
	Tank tank(70);
	int amount;
	do
	{
		tank.info();
		cout << "Enter fuel volume: "; cin >> amount;
		tank.fill(amount);
	} while (true);
#endif // TANK_CHECK
	
#ifdef ENGINE_CHECK
	Engine engine(3);
	engine.info();
#endif // ENGINE_CHECK

	Car bmw(10, 60, 250);
	bmw.control_car();





}