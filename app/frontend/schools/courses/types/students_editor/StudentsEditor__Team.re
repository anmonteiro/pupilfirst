type nonrec t = {
  id: string,
  name: string,
  coachIds: array(string),
  levelId: string,
  teamTags: array(string),
  students: array(StudentsEditor__Student.t),
};
let id = t => t.id;
let name = t => t.name;
let coachIds = t => t.coachIds;
let levelId = t => t.levelId;
let tags = t => t.teamTags;
let students = t => t.students;
let isSingleStudent = t => t.students |> Array.length == 1;
let make = (~id, ~name, ~teamTags, ~students, ~coachIds, ~levelId) => {
  id,
  name,
  teamTags,
  students,
  coachIds,
  levelId,
};
let makeFromJS = teamDetails =>
  Js.Array.map(
    ~f=
      team => {
        let students =
          Js.Array.map(
            ~f=
              studentDetails =>
                StudentsEditor__Student.makeFromJS(studentDetails),
            team##students,
          );
        let coachIds = Js.Array.map(~f=cids => cids, team##coachIds);
        make(
          ~id=team##id,
          ~name=team##name,
          ~teamTags=team##teamTags,
          ~levelId=team##levelId,
          ~students,
          ~coachIds,
        );
      },
    teamDetails,
  );
let update = (~name, ~teamTags, ~student, ~coachIds, ~team) => {
  let students =
    team.students
    |> Array.map(s =>
         if (s
             |> StudentsEditor__Student.id
             == (student |> StudentsEditor__Student.id)) {
           student;
         } else {
           s;
         }
       );
  {...team, name, teamTags, coachIds, students};
};
let replaceTeam = (team, teams) =>
  teams
  |> Array.map(t =>
       if (t.id == team.id) {
         team;
       } else {
         t;
       }
     );
let unsafeFind = (teams, componentName, teamId) =>
  teams
  |> ArrayUtils.unsafeFind(
       team => team.id == teamId,
       "Unable to find team with id: "
       ++ teamId
       ++ "in StudentdEditor__"
       ++ componentName,
     );
let updateStudent = (t, student) => {
  ...t,
  students:
    Js.Array.map(
      ~f=
        s =>
          if (StudentsEditor__Student.id(student)
              == StudentsEditor__Student.id(s)) {
            student;
          } else {
            s;
          },
      t.students,
    ),
};
