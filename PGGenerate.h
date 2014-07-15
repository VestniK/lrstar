
		#pragma once
		#include "ComGenerate.h"
		#include "PGCreateTables.h"

		class PGGenerate : public Generate, public PGCreateTables
		{
			public:
			static void  Initialize ();
			static void  EMIT     (int i);
			static void  EMITTYPE (int i);
			static void  EMITSTR  (int i);
			static int   VALUE    (int i);
		};
