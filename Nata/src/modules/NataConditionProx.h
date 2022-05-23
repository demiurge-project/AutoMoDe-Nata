/**
  * @file <src/modules/NataConditionProx.h>
  *
  * @author Ken H - <khasselm@ulb.ac.be>
  *
  * @package ARGoS3-Nata
  *
  * @license MIT License
  */

#ifndef NATA_CONDITION_PROX_H
#define NATA_CONDITION_PROX_H

#include "AutoMoDeCondition.h"

namespace argos {
	class NataConditionProx: public AutoMoDeCondition {
		public:
			NataConditionProx();
			virtual ~NataConditionProx();

			NataConditionProx(NataConditionProx* pc_condition);
			virtual NataConditionProx* Clone();

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
