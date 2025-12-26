import { motion } from "framer-motion";
import { CheckCircle2, Circle } from "lucide-react";
import { Card } from "@/components/ui/card";

interface StatusCardProps {
  message: string;
  index: number;
}

export function StatusCard({ message, index }: StatusCardProps) {
  return (
    <motion.div
      initial={{ opacity: 0, x: -20 }}
      animate={{ opacity: 1, x: 0 }}
      transition={{ delay: index * 0.1, duration: 0.4 }}
    >
      <Card className="p-4 flex items-center gap-3 hover:bg-muted/50 transition-colors border-border/50">
        <CheckCircle2 className="w-5 h-5 text-green-500" />
        <span className="font-mono text-sm text-foreground/80">{message}</span>
      </Card>
    </motion.div>
  );
}

export function EmptyStatusCard() {
  return (
    <div className="p-8 text-center border border-dashed rounded-lg text-muted-foreground bg-muted/30">
      <Circle className="w-8 h-8 mx-auto mb-2 opacity-50" />
      <p className="text-sm">No status messages received yet.</p>
    </div>
  );
}
