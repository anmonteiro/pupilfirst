exception InvalidSubmissionReportCompletionStatus;

type nonrec startedAt = Js.Date.t;
type nonrec queuedAt = Js.Date.t;

type nonrec conclusion =
  | Success
  | Failure
  | Error;

type nonrec completedTimestamps = {
  startedAt: Js.Date.t,
  completedAt: Js.Date.t,
};

type nonrec status =
  | Queued(queuedAt)
  | InProgress(startedAt)
  | Completed(completedTimestamps, conclusion);

type nonrec t = {
  id: string,
  status,
  testReport: option(string),
  queuedAt: Js.Date.t,
};

let decodeConclusion = conclusion =>
  switch (conclusion) {
  | `success => Success
  | `failure => Failure
  | `error => Error
  };

let decodeCompletedStatus = (startedAt, completedAt, conclusion) =>
  switch (startedAt, completedAt, conclusion) {
  | (None, _, _) =>
    Rollbar.critical(
      "Invalid completion status of submission report - start time missing for completed test",
    );
    raise(InvalidSubmissionReportCompletionStatus);
  | (_, None, _) =>
    Rollbar.critical(
      "Invalid completion status of submission report - end time missing for completed test",
    );
    raise(InvalidSubmissionReportCompletionStatus);
  | (_, _, None) =>
    Rollbar.critical(
      "Invalid completion status of submission report - conclusion missing for completed test",
    );
    raise(InvalidSubmissionReportCompletionStatus);
  | (Some(startedAt), Some(completedAt), Some(conclusion)) =>
    Completed(
      {
        startedAt: DateFns.decodeISO(startedAt),
        completedAt: DateFns.decodeISO(completedAt),
      },
      decodeConclusion(conclusion),
    )
  };

let decodeStatus = (status, conclusion, queuedAt, startedAt, completedAt) =>
  switch (status) {
  | `queued => Queued(DateFns.decodeISO(queuedAt))
  | `in_progress =>
    switch (startedAt) {
    | Some(startedAt) => InProgress(DateFns.decodeISO(startedAt))
    | None =>
      Rollbar.critical(
        "Invalid completion status of submission report - start time missing for test in progress",
      );
      raise(InvalidSubmissionReportCompletionStatus);
    }
  | `completed => decodeCompletedStatus(startedAt, completedAt, conclusion)
  };

let makeFromJS = obj => {
  id: obj##id,
  testReport: obj##testReport,
  queuedAt: obj##queuedAt->DateFns.decodeISO,
  status:
    decodeStatus(
      obj##status,
      obj##conclusion,
      obj##queuedAt,
      obj##startedAt,
      obj##completedAt,
    ),
};

let id = t => t.id;

let status = t => t.status;

let testReport = t => t.testReport;

let startedAt = t => t.startedAt;
