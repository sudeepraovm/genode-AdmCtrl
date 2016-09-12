/*
 * \brief  sched_controller is the user land controller for the kernel schedulers
 * \author Paul Nieleck
 * \date   2016/08/02
 *
 */

/* global includes */
#include <base/env.h>
#include <base/printf.h>
#include <base/rpc_server.h>
#include <base/sleep.h>
#include <cap_session/connection.h>
#include <root/component.h>

/* local includes */
#include <sched_controller_session/sched_controller_session.h>
#include <sched_controller/sched_controller.h>

namespace Sched_controller {

	struct Session_component : Genode::Rpc_object<Session>
	{
		void get_init_status() {
			PINF("sched_controller is initialized");
		}
	};

	class Root_component : public Genode::Root_component<Session_component>
	{
		protected:

			//Sched_controller::Session_component *_create_session(const char *args)
			Session_component *_create_session(const char *args)
			{
				return new(md_alloc()) Session_component();
			}

		public:

			Root_component(Genode::Rpc_entrypoint *ep,
			               Genode::Allocator *allocator)
			: Genode::Root_component<Session_component>(ep, allocator)
			{
				PDBG("Creating root component");
			}

	};

}

using namespace Genode;

int main(void)
{

	Sched_controller::Sched_controller ctr;

	Cap_connection cap;

	static Sliced_heap sliced_heap(env()->ram_session(),
	                               env()->rm_session());

	enum { STACK_SIZE = 4096 };
	static Rpc_entrypoint ep(&cap, STACK_SIZE, "sched_controller_ep");

	static Sched_controller::Root_component sched_controller_root(&ep, &sliced_heap);
	env()->parent()->announce(ep.manage(&sched_controller_root));

	sleep_forever();

	return 0;
}	
