type nonrec t = {
  id: string,
  name: string,
  description: option(string),
  endsAt: option(Js.Date.t),
  courseId: string,
};
let make = (~id, ~name, ~description, ~endsAt, ~courseId) => {
  id,
  name,
  description,
  endsAt,
  courseId,
};
let id = t => t.id;
let name = t => t.name;
let description = t => t.description;
let endsAt = t => t.endsAt;
let courseId = t => t.courseId;
let makeFromJs = cohort =>
  make(
    ~id=cohort##id,
    ~name=cohort##name,
    ~description=cohort##description,
    ~endsAt=cohort##endsAt->(Belt.Option.map(DateFns.decodeISO)),
  );
let filterValue = t => (t.id ++ ";") ++ t.name;
module Fragment = [%graphql
  {js|
  fragment CohortFragment on Cohort {
    id
    name
    description
    endsAt
    courseId
  }
|js}
];
let makeFromFragment = (cohort: Fragment.t) =>
  make(
    ~id=cohort.id,
    ~name=cohort.name,
    ~description=cohort.description,
    ~endsAt=cohort.endsAt->(Belt.Option.map(DateFns.decodeISO)),
    ~courseId=cohort.courseId,
  );
