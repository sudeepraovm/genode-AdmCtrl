/*
 * \brief  Scheduler Controller main class
 * \author Paul Nieleck
 * \date   2016\09\08
 *
 * This class is the main class of the scheduling
 * controller. It instantiates the cores, provides
 * interfaces, initiates task allocations and
 * is responsible for the controlling of cores.
 */

#ifndef _INCLUDE__SCHED_CONTROLLER__SCHED_CONTROLLER_H_
#define _INCLUDE__SCHED_CONTROLLER__SCHED_CONTROLLER_H_

#include <forward_list>
#include <unordered_map>
#include <vector>

#include "rq_manager_session/client.h"
#include "rq_manager_session/connection.h"
#include "sched_controller/pcore.h"

namespace Sched_controller
{

	struct Runqueue {

		Rq_manager::Task_class _task_class;
		Rq_manager::Task_strategy _task_strategy;
		int rq_buffer;

	};

	class Sched_controller
	{

		private:

			Rq_manager::Connection _rq_manager;
			int _num_rqs = 0;
			int _num_pcores = 0;
			Pcore *_pcore;                                                    /* Array of pcores */
			Runqueue *_runqueue;                                              /* Array of runqueues */
			std::unordered_multimap<Pcore*, Runqueue*> _pcore_rq_association; /* which pcore hosts which rq */

			int _set_num_pcores();
			int _init_pcores();
			int _init_runqueues();

		public:

			void allocate_task(Rq_manager::Rq_task);
			void task_to_rq(int, Rq_manager::Rq_task*);
			int get_num_rqs();
			void which_runqueues(std::vector<Runqueue>*, Rq_manager::Task_class, Rq_manager::Task_strategy);
			double get_utilization(int);
			std::forward_list<Pcore*> get_unused_cores();

			Sched_controller();
			~Sched_controller();

	};

}

#endif /* _INCLUDE__SCHED_CONTROLLER__SCHED_CONTROLLER_H_ */