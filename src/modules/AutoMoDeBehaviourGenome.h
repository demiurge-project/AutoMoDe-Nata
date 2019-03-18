/**
  * @file <src/modules/AutoMoDeBehaviourGenome.h>
  *
  * @package ARGoS3-AutoMoDe
  *
  * @license MIT License
  */

#ifndef AUTOMODE_BEHAVIOUR_GENOME_H
#define AUTOMODE_BEHAVIOUR_GENOME_H

#include "AutoMoDeBehaviour.h"

namespace argos {
	class AutoMoDeBehaviourGenome: public AutoMoDeBehaviour {
		public:
			AutoMoDeBehaviourGenome();
			AutoMoDeBehaviourGenome(AutoMoDeBehaviourGenome* pc_behaviour);
			virtual ~AutoMoDeBehaviourGenome();

			virtual void ControlStep();
			virtual void Reset();
			virtual void ResumeStep();
			virtual void Init();

			virtual AutoMoDeBehaviourGenome* Clone();
		private:
			static int INDEX;
	};
}

#endif
