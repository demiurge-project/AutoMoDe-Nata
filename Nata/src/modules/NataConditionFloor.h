/**
  * @file <src/modules/NataConditionFloor.h>
  *
  * @author Ken H - <khasselm@ulb.ac.be>
  *
  * @package ARGoS3-Nata
  *
  * @license MIT License
  */

#ifndef NATA_CONDITION_FLOOR_H
#define NATA_CONDITION_FLOOR_H

#include "AutoMoDeCondition.h"

namespace argos {
	class NataConditionFloor: public AutoMoDeCondition {
		public:
			NataConditionFloor();
			virtual ~NataConditionFloor();

			NataConditionFloor(NataConditionFloor* pc_condition);
			virtual NataConditionFloor* Clone();

			virtual bool Verify();
			virtual void Reset();
			virtual void Init();

		private:
			UInt32 m_fGroundColor;
			CRange<Real> m_fGroundThresholdRange;
			Real m_fProbability;
	};
}

#endif
