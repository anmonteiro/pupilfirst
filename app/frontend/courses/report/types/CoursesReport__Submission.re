type targetRole =
  | Student
  | Team(array(studentId))
and studentId = string;
type nonrec t = {
  id: string,
  title: string,
  createdAt: Js.Date.t,
  status: [ | `PendingReview | `Rejected | `Completed],
  levelId: string,
  targetId: string,
  targetRole,
};
let make =
    (~id, ~title, ~createdAt, ~levelId, ~status, ~targetId, ~targetRole) => {
  id,
  title,
  createdAt,
  status,
  levelId,
  targetId,
  targetRole,
};
let id = t => t.id;
let levelId = t => t.levelId;
let title = t => t.title;
let status = t => t.status;
let createdAt = t => t.createdAt;
let targetId = t => t.targetId;
let targetRole = t => t.targetRole;
let createdAtPretty = t => t.createdAt->(DateFns.format("MMMM d, yyyy"));
let makeFromJs = submissions =>
  Js.Array.map(
    ~f=submission => {
      let createdAt = DateFns.decodeISO(submission##createdAt);
      let status =
        switch (submission##passedAt) {
        | Some(_passedAt) => `Completed
        | None =>
          switch (submission##evaluatedAt) {
          | Some(_time) => `Rejected
          | None => `PendingReview
          }
        };
      let targetRole =
        if (submission##teamTarget) {
          Team(submission##studentIds);
        } else {
          Student;
        };
      make(
        ~id=submission##id,
        ~title=submission##title,
        ~createdAt,
        ~levelId=submission##levelId,
        ~targetId=submission##targetId,
        ~targetRole,
        ~status,
      );
    },
    submissions,
  );
