/**
 * @file adrc.h
 *
 * Definition of generic ADRC controller.
 *
 * @author Sunyi
 */

#ifndef FIRST_H_
#define FIRST_H_
#include <stdint.h>
#include<px4_platform_common/module.h>
#include <px4_platform_common/module_params.h>
#include<msg/tmp/headers/actuator_controls.h>

__BEGIN_DECLS

class First : public ModuleBase<First>, public ModuleParams
{
public:
	First();
	~First() override;
	static int task_spawn(int argc, char*argv[]);
	void run()override;
	static First *instantiate(int argc, char *argv[]);
	static int custom_command(int argc, char *argv[]);
	static int print_usage(const char *reason = nullptr);

	orb_advert_t _actuators_2_pub;
	orb_id_t _actuators_id2;
	bool	_actuators_2_circuit_breaker_enabled;
	struct actuator_controls_s	_actuators2;

};
__END_DECLS
#endif
