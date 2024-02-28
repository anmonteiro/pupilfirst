type nonrec t = {
  id: string,
  name: string,
  students: array(UserProxy.t),
  cohort: Cohort.t,
};
let id = t => t.id;
let name = t => t.name;
let students = t => t.students;
let cohort = t => t.cohort;
let make = (~id, ~name, ~students, ~cohort) => {id, name, students, cohort};
module Fragment = [%graphql
  {js|
   fragment TeamFragment on Team {
    id
    name
    students {
      id
      user {
        id
        name
        avatarUrl
        fullTitle
      }
    }
    cohort {
      id
      name
      description
      endsAt
      courseId
    }
  }

|js}
];
let makeFromFragment = (team: Fragment.t) =>
  make(
    ~id=team.id,
    ~name=team.name,
    ~students=
      team.students
      ->(
          Js.Array.map(~f=(s: Fragment.t_students) =>
            UserProxy.make(
              ~id=s.id,
              ~name=s.user.name,
              ~avatarUrl=s.user.avatarUrl,
              ~fullTitle=s.user.fullTitle,
              ~userId=s.user.id,
            )
          )
        ),
    ~cohort=
      Cohort.make(
        ~id=team.cohort.id,
        ~name=team.cohort.name,
        ~description=team.cohort.description,
        ~endsAt=team.cohort.endsAt->(Belt.Option.map(DateFns.decodeISO)),
        ~courseId=team.cohort.courseId,
      ),
  );
