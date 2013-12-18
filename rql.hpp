#include "proto\ql2.pb.h"
#include <vector>
#include "exception.hpp"
#include "connection.hpp"

#ifndef RETHINK_DB_DRIVER_RQL
#define RETHINK_DB_DRIVER_RQL

using namespace std;
using namespace com::rethinkdb;

namespace com {
	namespace rethinkdb {
		namespace driver {
			
			class RQL_Array;
			class RQL_Object;
			class RQL_String;
			class RQL_Database;
			class RQL { // "Top" according to photobuf specification
			public:			

				RQL::RQL() : term(Term()) {};

				vector<shared_ptr<Response>> RQL::run(shared_ptr<connection> conn);				

				/* -------------------------------------------------------------------- */

				shared_ptr<RQL_Database> db(shared_ptr<RQL_String> db_name);
				shared_ptr<RQL_Database> db(const string& db_name);
				shared_ptr<RQL_Array> db_list();
				shared_ptr<RQL_Object> db_create(shared_ptr<RQL_String> db_name);
				shared_ptr<RQL_Object> db_create(const string& db_name);
				shared_ptr<RQL_Object> db_drop(shared_ptr<RQL_String> db_name);
				shared_ptr<RQL_Object> db_drop(const string& db_name);

				/* -------------------------------------------------------------------- */

				Term term;

			protected:				

				void check_response(shared_ptr<Response> response);

			};

			class RQL_Datum : public RQL {};
			class RQL_String : public RQL_Datum {
			public:
				RQL_String(const string& str) {
					term.set_type(Term::TermType::Term_TermType_DATUM);
					term.mutable_datum()->set_type(Datum::DatumType::Datum_DatumType_R_STR);
					term.mutable_datum()->set_r_str(str);
				}
			};
			class RQL_Array : public RQL_Datum {};
			class RQL_Object : public RQL_Datum {};

			class RQL_Database : public RQL {
			public:
				shared_ptr<RQL_Array> table_list() {
					shared_ptr<RQL_Array> array(new RQL_Array());
					array->term.set_type(Term::TermType::Term_TermType_TABLE_LIST);
					*(array->term.add_args()) = this->term;
					return array;
				}
			};
		}
	}
}

#endif