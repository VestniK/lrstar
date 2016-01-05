
		#pragma once
		#include "ComGenerate.h"
		#include "LGCreateTables.h"

		class LGGenerate : public Generate, public LGCreateTables
		{
			public:
			static void  Initialize ();
			static void  EMIT     (int i);
			static void  EMITTYPE (int i);
			static void  EMITSTR  (int i);
			static int   VALUE    (int i);
		};
