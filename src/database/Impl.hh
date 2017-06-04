<?hh
require_once("../src/database/Database.hh");
require_once("../src/database/mongo/MongoDatabase.hh");

function DB(): Database {
  return new MongoDatabase();
}
