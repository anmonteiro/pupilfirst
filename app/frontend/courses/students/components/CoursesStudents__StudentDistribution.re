[%%raw {js|import "./CoursesStudents__StudentDistribution.css"|js}];
open CoursesStudents__Types;
let str = React.string;
let tr = I18n.t(~scope="components.CoursesStudents__StudentDistribution");
let ts = I18n.t(~scope="shared");
let stylingForLevelPills = percentageStudents => {
  let emptyStyle = ReactDOM.Style.make();
  let styleWithWidth =
    ReactDOM.Style.make(
      ~width=percentageStudents->Js.Float.toString ++ "%",
      (),
    );
  if (0.0 == percentageStudents) {
    ("w-8 flex-grow", emptyStyle, "bg-green-200 text-green-800");
  } else if (0.0 <= percentageStudents && percentageStudents < 5.0) {
    ("w-8 flex-shrink-0", emptyStyle, "bg-green-200 text-green-800");
  } else if (5.0 <= percentageStudents && percentageStudents < 20.0) {
    ("", styleWithWidth, "bg-green-300 text-green-800");
  } else if (20.0 <= percentageStudents && percentageStudents < 40.0) {
    ("", styleWithWidth, "bg-green-400 text-green-900");
  } else if (40.0 <= percentageStudents && percentageStudents < 60.0) {
    ("", styleWithWidth, "bg-green-500 text-white");
  } else if (60.0 <= percentageStudents && percentageStudents < 80.0) {
    ("", styleWithWidth, "bg-green-600 text-white");
  } else {
    ("", styleWithWidth, "bg-green-700 text-white");
  };
};
let onLevelSelect = (value, params, href) =>
  switch (params) {
  | Some(p) =>
    Webapi.Url.URLSearchParams.set("level", value, p);
    ReasonReactRouter.push("?" ++ Webapi.Url.URLSearchParams.toString(p));
  | None =>
    let search = Webapi.Dom.location->Webapi.Dom.Location.search;
    let params = Webapi.Url.URLSearchParams.make(search);
    Webapi.Url.URLSearchParams.set("level", value, params);
    let currentPath = Webapi.Dom.location->Webapi.Dom.Location.pathname;
    let searchString = Webapi.Url.URLSearchParams.toString(params);
    let path = Belt.Option.getWithDefault(href, currentPath);
    Webapi.Dom.window->(
                         Webapi.Dom.Window.setLocation(
                           (path ++ {js|?|js}) ++ searchString,
                         )
                       );
  };
let studentDistributionSkeleton =
  <div className="skeleton-body-container w-full mx-auto">
    <div className="skeleton-body-wrapper px-3 lg:px-0">
      <div className="flex">
        <div className="w-1/6">
          <div
            className="skeleton-placeholder__line-sm skeleton-animate w-6 mx-auto"
          />
          <div
            className="skeleton-placeholder__line-md skeleton-animate mt-2"
          />
        </div>
        <div className="w-5/12">
          <div
            className="skeleton-placeholder__line-sm skeleton-animate w-6 mx-auto"
          />
          <div
            className="skeleton-placeholder__line-md skeleton-animate mt-2"
          />
        </div>
        <div className="w-1/4">
          <div
            className="skeleton-placeholder__line-sm skeleton-animate w-6 mx-auto"
          />
          <div
            className="skeleton-placeholder__line-md skeleton-animate mt-2"
          />
        </div>
        <div className="w-1/12">
          <div
            className="skeleton-placeholder__line-sm skeleton-animate w-6 mx-auto"
          />
          <div
            className="skeleton-placeholder__line-md skeleton-animate mt-2"
          />
        </div>
        <div className="w-1/12">
          <div
            className="skeleton-placeholder__line-sm skeleton-animate w-6 mx-auto"
          />
          <div
            className="skeleton-placeholder__line-md skeleton-animate mt-2"
          />
        </div>
      </div>
    </div>
  </div>;
[@react.component]
let make = (~studentDistribution, ~params=?, ~href=?) =>
  <div ariaLabel="Students level-wise distribution" className="w-full py-4">
    {let totalStudentsInCourse =
       studentDistribution->(
                              Js.Array.reduce(
                                ~f=(x, y) =>
                                  x + DistributionInLevel.studentsInLevel(y),
                                ~init=0,
                              )
                            );
     let completedLevels =
       DistributionInLevel.levelsCompletedByAllStudents(studentDistribution);
     <div className="flex w-full border bg-gray-50 rounded font-semibold ">
       {studentDistribution
        ->(
            Js.Array.filter(~f=level => DistributionInLevel.number(level) != 0)
          )
        ->DistributionInLevel.sort
        ->(
            Js.Array.map(~f=level => {
              let percentageStudents =
                DistributionInLevel.percentageStudents(
                  level,
                  totalStudentsInCourse,
                );
              let (pillClass, style, pillColor) =
                stylingForLevelPills(percentageStudents);
              let tip =
                <div className="text-left">
                  <p>
                    {LevelLabel.format(
                       DistributionInLevel.number(level)->string_of_int,
                     )
                     ->str}
                  </p>
                  <p>
                    {(
                       (ts("students") ++ ": ")
                       ++ DistributionInLevel.studentsInLevel(level)
                          ->string_of_int
                     )
                     ->str}
                  </p>
                  <p>
                    {(
                       (ts("percentage") ++ ": ")
                       ++ percentageStudents->(
                                                Js.Float.toFixed(
                                                  ~digits=1,
                                                )
                                              )
                     )
                     ->str}
                  </p>
                </div>;
              <div
                key={DistributionInLevel.id(level)}
                ariaLabel={
                  "Students in level "
                  ++ DistributionInLevel.number(level)->string_of_int
                }
                className={
                  "student-distribution__container text-center relative focus-within:outline-none focus-within:opacity-75 "
                  ++ pillClass
                }
                style>
                <label
                  htmlFor={
                    tr("students_level")
                    ++ DistributionInLevel.number(level)->string_of_int
                  }
                  className="absolute -mt-5 left-0 right-0 inline-block text-xs text-gray-600 text-center">
                  {level->DistributionInLevel.shortName->str}
                </label>
                <Tooltip className="w-full" tip position=`Bottom>
                  <button
                    id={
                      tr("students_level")
                      ++ DistributionInLevel.number(level)->string_of_int
                    }
                    onClick={_ =>
                      onLevelSelect(
                        DistributionInLevel.filterName(level),
                        params,
                        href,
                      )
                    }
                    className={
                      "student-distribution__pill w-full hover:shadow-inner focus:shadow-inner relative cursor-pointer border-white text-xs leading-none text-center "
                      ++ (
                        if (completedLevels->(Js.Array.includes(~value=level))) {
                          "bg-yellow-300 text-yellow-900";
                        } else {
                          DistributionInLevel.unlocked(level)
                            ? pillColor
                            : "student-distribution__pill--locked cursor-default bg-gray-300"
                              ++ " text-gray-800";
                        }
                      )
                    }>
                    {if (completedLevels->(Js.Array.includes(~value=level))) {
                       <PfIcon className="if i-check-solid text-tiny" />;
                     } else {
                       <div>
                         <div
                           className={
                                       if (level->DistributionInLevel.unlocked) {
                                         "";
                                       } else {
                                         "student-distribution__team-count-value";
                                       }
                                     }>
                           {level
                            ->DistributionInLevel.studentsInLevel
                            ->string_of_int
                            ->str}
                         </div>
                         {if (level->DistributionInLevel.unlocked) {
                            React.null;
                          } else {
                            <div className="student-distribution__locked-icon">
                              <i className="fas fa-lock text-tiny" />
                            </div>;
                          }}
                       </div>;
                     }}
                  </button>
                </Tooltip>
              </div>;
            })
          )
        ->React.array}
     </div>}
  </div>;
let makeFromJson = props => {
  open Json.Decode;
  let studentDistribution =
    field("studentDistribution", array(DistributionInLevel.decode), props);
  let href = optional(field("href", string), props);
  make([%obj {studentDistribution, params: None, href}]);
};
