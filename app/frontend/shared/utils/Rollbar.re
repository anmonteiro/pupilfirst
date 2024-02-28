[@mel.scope "Rollbar"] external critical: string => unit = "critical";
[@mel.scope "Rollbar"] external error: string => unit = "error";
[@mel.scope "Rollbar"] external warning: string => unit = "warning";
[@mel.scope "Rollbar"] external info: string => unit = "info";
[@mel.scope "Rollbar"] external debug: string => unit = "debug";
