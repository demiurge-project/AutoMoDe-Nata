/**
  * @file <src/modules/NataConditionLight.h>
  *
  * @author Ken H - <khasselm@ulb.ac.be>
  *
  * @package ARGoS3-Nata
  *
  * @license MIT License
  */

#ifndef NATA_CONDITION_LIGHT_H
#define NATA_CONDITION_LIGHT_H

#include "AutoMoDeCondition.h"

namespace argos {
	class NataConditionLight: public AutoMoDeCondition {
		public:
			NataConditionLight();
			virtual ~NataConditionLight();

			NataConditionLight(NataConditionLight* pc_condition);
			virtual NataConditionLight* Clone();

			virtual bool Verify();
			virtual void Reset();
			virtual void Init();

		private:
            Real m_fThreshold;
			Real m_fProbability;
			UInt32 m_unSide;
	};
}

#endif
