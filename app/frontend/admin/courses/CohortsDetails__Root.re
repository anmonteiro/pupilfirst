let str = React.string;
let t = I18n.t(~scope="components.CohortsDetails__Root");
let pageLinks = cohortId => [|
  School__PageHeader.makeLink(
    ~href=({js|/school/cohorts/|js} ++ cohortId) ++ {js|/details|js},
    ~title="Details",
    ~icon="fas fa-edit",
    ~selected=true,
    (),
  ),
  School__PageHeader.makeLink(
    ~href=({js|/school/cohorts/|js} ++ cohortId) ++ {js|/actions|js},
    ~title="Actions",
    ~icon="fas fa-cog",
    ~selected=false,
    (),
  ),
|];
type nonrec state =
  | Unloaded
  | Loading
  | Loaded(Cohort.t)
  | Errored;
module CohortFragment = Cohort.Fragment;
module CohortDetailsDataQuery = [%graphql
  {js|
  query CohortDetailsDataQuery($id: ID!) {
    cohort(id: $id) {
      ...CohortFragment
    }
  }
|js}
];
let loadData = (id, setState, setCourseId) => {
  setState(_ => Loading);
  CohortDetailsDataQuery.fetch(~notifyOnNotFound=false, {id: id})
  |> Js.Promise.then_((response: CohortDetailsDataQuery.t) => {
       setState(_ => Loaded(response.cohort->Cohort.makeFromFragment));
       setCourseId(response.cohort.courseId);
       Js.Promise.resolve();
     })
  |> Js.Promise.catch(_error => {
       setState(_ => Errored);
       Js.Promise.resolve();
     })
  |> ignore;
};
[@react.component]
let make = (~cohortId) => {
  let (state, setState) = React.useState(() => Unloaded);
  let courseContext = React.useContext(SchoolRouter__CourseContext.context);
  React.useEffect1(
    () => {
      loadData(cohortId, setState, courseContext.setCourseId);
      None;
    },
    [|cohortId|],
  );

  switch (state) {
  | Unloaded
  | Loading => SkeletonLoading.coursePage()
  | Loaded(cohort) =>
    let courseId = Cohort.courseId(cohort);
    <div>
      <School__PageHeader
        exitUrl={({js|/school/courses/|js} ++ courseId) ++ {js|/cohorts|js}}
        title={t(
          "page_title",
          ~variables=[|("cohort_name", Cohort.name(cohort))|],
        )}
        description={t("page_description")}
        links={pageLinks(cohortId)}
      />
      <AdminCoursesShared__CohortsEditor courseId cohort />
    </div>;
  | Errored => <ErrorState />
  };
};
